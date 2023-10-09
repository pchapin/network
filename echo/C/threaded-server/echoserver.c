/*!
 * \file echoserver.c
 * \author Peter C. Chapin <pchapin@vtc.edu>
 * \brief Implementation of a multi-threaded concurrent echo server in C.
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#ifndef S_SPLINT_S    // Workaround for splint.
#include <unistd.h>
#endif

#define BUFFER_SIZE 128
#define PID_FILE    "dtserver.pid"


//! Structure to hold the thread parameters (needed for listening and accepting connections).
struct ListenParameters {
    int listen_handle;       //!< Handle to an open socket for accepting clients.
};

//! Structure to hold the thread parameters (needed to service a connection).
struct ConnectionParameters {
    int connection_handle;   //!< Handle to an open socket connected to the client.
};


//! Create a file containing the process ID of the server.
/*!
 * This function creates a file containing just the process ID number of the server. This file
 * can be used in scripts to easily determin the server's ID (for example to send it a signal).
 * There is no indication if this function fails to create the PID file.
 *
 * \todo If a PID file exists already that suggests either a) the previous server crashed, or
 * b) the previous server is still running. Either way some sort of special action should be
 * taken.
 */
static void create_pid_file( )
{
    pid_t  my_process = getpid( );
    FILE  *pid_file;

    if( ( pid_file = fopen( PID_FILE, "w" ) ) != NULL ) {
        fprintf( pid_file, "%d\n", (int)my_process );
        fclose( pid_file );
    }
}


//! Provides service for a single client.
/*!
 * This function is called in a separate detached thread for each client. It does not return
 * any indication of its success or failure, nor does it attempt to log anything. This function
 * does close the client connection when it is finished with it.
 *
 * \param arg A pointer to a ConnectionParameters structure containing information about the
 * particular client connection to be serviced. This structure must be dynamically allocated.
 * This function takes responsibility for deallocating the structure.
 */
static /*@null@*/ void *service_connection( /*@only@*/ void *arg )
{
    char    buffer[BUFFER_SIZE]; // General purpose buffer.
    ssize_t buffer_length;       // Number of characters in buffer.
    struct  ConnectionParameters *parameters = (struct ConnectionParameters *)arg;

    // Perform the requested service. (echo the received data back to the client)
    // TODO: What should be done about possible deadlock between client and server?
    while ((buffer_length = read(parameters->connection_handle, buffer, BUFFER_SIZE)) > 0) {
        write(parameters->connection_handle, buffer, buffer_length);
    }

    // Close this connection.
    close( parameters->connection_handle );
    free( parameters );
    return NULL;
}


//! Accepts clients and creates a thread to handle each.
/*!
 * This function loops infinitely accepting clients and creating detached thread for each
 * client. The only way to exit this function is to cancel the thread that is running it.
 *
 * \param arg A pointer to a ListenParameters structure containing information about the socket
 * to listen for new connections. This function does not free() this structure; if that is
 * needed the caller must do it.
 */
static /*@null@*/ void *handle_clients( /*@only@*/ void *arg )
{
    struct sockaddr_in  client_address;      // Stores address of client.
    socklen_t           client_length;       // Size of client_address structure.
    int                 connection_handle;   // Handle representing a connection.
    char                buffer[BUFFER_SIZE]; // Buffer used to hold informational messages.
    pthread_t           thread_ID;           // Thread processing a connection.
    int                 thread_result;       // Return value from pthread_create.
    struct ListenParameters     *l_parameters = (struct ListenParameters *)arg;
    struct ConnectionParameters *c_parameters;

   // Accept connections forever.
    while( 1 ) {
        memset( &client_address, 0, sizeof(client_address) );
        client_length = sizeof(client_address);

        // Block until a client comes along.
        connection_handle = accept( l_parameters->listen_handle, (struct sockaddr *) &client_address, &client_length );

        // Check for errors from accept.
        if( connection_handle == -1 ) {
            perror( "Accept failed" );
        }
        else {
            // Display informational message.
            inet_ntop( AF_INET, &client_address.sin_addr, buffer, BUFFER_SIZE );
            printf( "Accepted client connection from: %s\n", buffer );

            // Create a dynamically allocated instance of struct ConnectionParameters.
            c_parameters = malloc( sizeof(struct ConnectionParameters) );
            if( c_parameters == NULL ) {
                fprintf( stderr, "Out of memory when servicing connection!\n" );
            }
            else {
                // Start the thread to service the client. If successful, detach that thread.
                c_parameters->connection_handle = connection_handle;
                thread_result = pthread_create( &thread_ID, NULL, service_connection, c_parameters );
                if( thread_result != 0 ) {
                    free( c_parameters );
                    fprintf( stderr, "Unable to create thread to service connection!" );
                }
                else {
                    pthread_detach( thread_ID );
                }
            }
        }
    }
    return NULL;
}


int main( int argc, char **argv )
{
    struct sockaddr_in  server_address;      // Defines listening address.
    unsigned short      port = 13;           // Listening port number.
    int                 listen_handle;       // Handle of accepting socket.
    pthread_t           client_handler_ID;   // ID of thread to accept client connections.
    sigset_t            signal_set;          // Set of signals to block/process.
    int                 signal_number;       // Currently active signal number.
    struct ListenParameters *parameters;

    // Do we have an explicit port address? If so, override the default.
    if( argc == 2 ) {
        port = atoi( argv[1] );
    }

    // Create the server socket as an IPv4 TCP socket.
    if( ( listen_handle = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
        perror( "Unable to create socket" );
        return EXIT_FAILURE;
    }

    // Prepare the server socket address structure.
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = htonl( INADDR_ANY );
    server_address.sin_port        = htons( port );

    // Bind the server socket.
    if( bind( listen_handle, (struct sockaddr *) &server_address, sizeof(server_address) ) == -1 ) {
        perror( "Unable to bind socket" );
        close( listen_handle );
        return EXIT_FAILURE;
    }

    // Allow incoming connections.
    if( listen( listen_handle, 32 ) == -1 ) {
        perror( "Unable to listen" );
        close( listen_handle );
        return EXIT_FAILURE;
    }

    // Create a ListenParameters structure.
    if( ( parameters = malloc( sizeof(struct ListenParameters) ) ) == NULL ) {
        fprintf( stderr, "Out of memory when creating the listening thread!\n" );
        close( listen_handle );
        return EXIT_FAILURE;
    }
    parameters->listen_handle = listen_handle;

    create_pid_file( );

    // Block all signals in this thread and (by default) the threads it creates.
    sigfillset( &signal_set );
    pthread_sigmask( SIG_SETMASK, &signal_set, NULL );

    // Create a thread to accept client connections and deal with them.
    // TODO: We should verify that this successfully creates the thread.
    pthread_create( &client_handler_ID, NULL, handle_clients, parameters );

    // The main thread now waits for any signal and process it.
    while( 1 ) {
        sigwait( &signal_set, &signal_number );
        switch( signal_number ) {
            case SIGINT:
            case SIGTERM:
                printf( "\ndtserver cleaning up...\n" );
                pthread_cancel( client_handler_ID );       // Cancel the listening thread.
                pthread_join( client_handler_ID, NULL );   // Wait for it to stop.
                close( listen_handle );                    // Close the listening socket.
                free( parameters );                        // Release parameters used by listening thread.
                // TODO: Shutdown active clients gracefully.
                remove( PID_FILE );                        // Remove PID file.
                exit( EXIT_SUCCESS );
            default:
                printf( "\nUNEXPECTED SIGNAL: %d\n", signal_number );
        }
    }
}
