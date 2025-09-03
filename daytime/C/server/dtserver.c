/*!
 * \file  dtserver.c
 * \author Peter Chapin
 * \brief Implementation of a simple daytime server in C.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 128

int main( int argc, char **argv )
{
    int                 listen_handle;       // Handle of accepting socket.
    int                 connection_handle;   // Handle representing a connection.
    struct sockaddr_in  server_address;      // Defines listening address.
    unsigned short      port = 13;           // Listening port number.
    struct sockaddr_in  client_address;      // Stores address of client.
    socklen_t           client_length;       // Size of client_address structure.
    char                buffer[BUFFER_SIZE]; // General purpose buffer.

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
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = htonl( INADDR_ANY );
    server_address.sin_port        = htons( port );

    // Bind the server socket.
    if( bind( listen_handle, (struct sockaddr *)&server_address, sizeof(server_address) ) == -1 ) {
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

    // Accept connections forever.
    while( 1 ) {
        memset( &client_address, 0, sizeof(client_address) );
        client_length = sizeof(client_address);

        // Block until a client comes along.
        connection_handle =
            accept( listen_handle, (struct sockaddr *)&client_address, &client_length );

        // Check for errors from accept.
        if( connection_handle == -1 ) {
            perror( "Accept failed" );
        }
        else {
            // Display informational message.
            inet_ntop( AF_INET, &client_address.sin_addr, buffer, BUFFER_SIZE );
            printf( "Accepted client connection from: %s\n", buffer );

            // Perform the requested service. (Send the current time of day)
            time_t ticks = time( NULL );
            snprintf( buffer, BUFFER_SIZE, "%.24s\r\n", ctime( &ticks ) );
            size_t buffer_length = strlen( buffer );

            // Send the response to the client.
            // It is necessary to do this in a loop to ensure the entire response is sent.
            size_t total = 0;
            while( total < buffer_length ) {
                ssize_t length_written = write( connection_handle, buffer + total, buffer_length - total );
                if( length_written == -1 ) {
                    // Checking for EINTR is overkill for this program, but we'll want to deal with it later.
                    if( errno == EINTR ) continue; // Retry if interrupted.
                    perror( "Write failed" );
                    break;
                }
                total += (size_t)length_written;
            }

            // Close this connection.
            close( connection_handle );
        }
    }
}
