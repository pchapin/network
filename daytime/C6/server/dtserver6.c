/*****************************************************************************
FILE   : dtserver6.c
SUBJECT: Implementation of an iterative daytime server in C for IPv6.

Please send comments or bug reports to

     Peter C. Chapin
     Computer Information Systems
     Vermont Technical College
     Williston, VT
     pchapin@vtc.edu
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#ifndef S_SPLINT_S    // Workaround for splint.
#include <unistd.h>
#endif

#define BUFFER_SIZE 128

int main( int argc, char **argv )
{
    int                 listen_handle;
    int                 connection_handle;
    struct sockaddr_in6 server_address;
    unsigned short      port = 13;
    struct sockaddr_in6 client_address;
    socklen_t           client_length;
    char                buffer[BUFFER_SIZE];
    size_t              buffer_length;
    time_t              ticks;

    // Do we have an explicit port address?
    if( argc == 2 ) {
        port = atoi( argv[1] );
    }

    // Create the server socket as an IPv6 TCP socket.
    if( ( listen_handle = socket( PF_INET6, SOCK_STREAM, 0 ) ) < 0 ) {
        perror( "Unable to create socket" );
        return EXIT_FAILURE;
    }

    // Prepare the server socket address structure.
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin6_family = AF_INET6;
    server_address.sin6_addr   = in6addr_any;
    server_address.sin6_port   = htons( port );

    // Bind the server socket.
    if( bind( listen_handle, (struct sockaddr *) &server_address, sizeof(server_address) ) < 0 ) {
        perror( "Unable to bind listening address" );
        close( listen_handle );
        return EXIT_FAILURE;
    }

    // Allow incoming connections.
    if( listen( listen_handle, 32 ) < 0 ) {
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
            accept( listen_handle, (struct sockaddr *) &client_address, &client_length );

        // Check for errors from accept.
        if( connection_handle < 0 ) {
            perror( "Accept failed" );
        }
        else {
            // Display informational message.
            inet_ntop( AF_INET6, &client_address.sin6_addr, buffer, BUFFER_SIZE );
            printf( "Accepted client connection from: %s\n", buffer );

            // Perform the requested service. (Send the current time of day)
            ticks = time( NULL );
            snprintf( buffer, BUFFER_SIZE, "%.24s\r\n", ctime( &ticks ) );
            buffer_length = strlen( buffer );
            if( write( connection_handle, buffer, buffer_length ) != buffer_length ) {
                perror( "Write failed" );
            }

            // Close this connection.
            close( connection_handle );
        }
    }
}
