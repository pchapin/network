/*!
 * \file echoserver.c
 * \author Peter Chapin
 * \brief Implementation of a concurrent echo server in C.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 128

void SIGCHLD_handler( int __attribute__((unused)) signal_number )
{
    int status;
    while( waitpid( -1, &status, WNOHANG ) > 0 ) /* null */ ;
}


int main( int argc, char **argv )
{
    int                 listen_handle;       // Handle of accepting socket.
    int                 connection_handle;   // Handle representing a connection.
    struct sockaddr_in  server_address;      // Defines listening address.
    unsigned short      port = 7;            // Listening port number.
    struct sockaddr_in  client_address;      // Stores address of client.
    socklen_t           client_length;       // Size of client_address structure.
    char                buffer[BUFFER_SIZE]; // General purpose buffer.
    ssize_t             buffer_length;       // Number of characters in buffer.
    struct sigaction    action;              // Structure for defining signal handling.
    pid_t               child_id;            // Process ID number of a child process.

    // Do we have an explicit port address? If so, override the default.
    // TODO: Add error handling to ensure the port number is valid.
    if( argc == 2 ) {
        port = (unsigned short)atoi( argv[1] );
    }

    // Create the server socket as an IPv4 TCP socket.
    if( ( listen_handle = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
        perror( "Unable to create socket" );
        return EXIT_FAILURE;
    }

    // Prepare the server socket address structure.
    memset( &server_address, 0, sizeof( server_address ) );
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = htonl( INADDR_ANY );
    server_address.sin_port        = htons( port );

    // Bind the server socket.
    if( bind( listen_handle, (struct sockaddr *) &server_address, sizeof( server_address ) ) == -1 ) {
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

    action.sa_handler = SIGCHLD_handler;
    sigemptyset( &action.sa_mask );
    action.sa_flags = 0;
    sigaction( SIGCHLD, &action, NULL );

    // Accept connections forever.
    while( 1 ) {
        memset( &client_address, 0, sizeof( client_address ) );
        client_length = sizeof( client_address );

        // Block until a client comes along.
        while( ( connection_handle =
                    accept( listen_handle, (struct sockaddr *) &client_address, &client_length ) ) == -1 ) {

            // Check for errors from accept. Interrupted system calls are not errors.
            if( errno != EINTR ) {
                perror( "Accept failed" );
            }
        }

        // Display informational message.
        inet_ntop( AF_INET, &client_address.sin_addr, buffer, BUFFER_SIZE );
        printf( "Accepted client connection from: %s\n", buffer );

        if( ( child_id = fork( ) ) == -1 ) {
            perror( "Fork failed" );
        }
        else if( child_id == 0 ) {
            // We are the child process...
            close( listen_handle );

            // Perform the requested service. (echo the received data back to the client)
            // TODO: What should be done about possible deadlock between client and server?
            // TODO: Handle errors returned from `read` and `write`.
            while( ( buffer_length = read( connection_handle, buffer, BUFFER_SIZE ) ) > 0 ) {
                write( connection_handle, buffer, (size_t)buffer_length );
            }
            close( connection_handle );
            exit( EXIT_SUCCESS );
        }

        // Close the connection handle in the parent.
        close( connection_handle );
    }
}
