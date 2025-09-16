/*!
 * \file  dtclient6.c
 * \author Peter Chapin
 * \brief Implementation of a simple daytime client for IPv6 in C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 128

int main( int argc, char **argv )
{
    int                 socket_handle;
    ssize_t             count;
    char                buffer[BUFFER_SIZE];
    struct sockaddr_in6 server_address;
    unsigned short      port = 13;

    // Do we have a command line argument?
    if( argc < 2 ) {
        fprintf( stderr, "usage: %s IPAddress [port]\n", argv[0] );
        return EXIT_FAILURE;
    }

    // Do we have an explicit port number? If so, override the default.
    // TODO: Add error handling to ensure the port number is valid.
    if( argc == 3 ) {
        port = (unsigned short)atoi( argv[2] );
    }

    // Create a socket.
    if( ( socket_handle = socket( PF_INET6, SOCK_STREAM, 0 ) ) < 0 ) {
        perror( "Unable to create socket" );
        return EXIT_FAILURE;
    }

    // Set up the server address.
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port   = htons( port );
    if( inet_pton( AF_INET6, argv[1], &server_address.sin6_addr ) <= 0 ) {
        fprintf( stderr, "Unable to convert address into binary form\n" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    // If this is a link-local address...
    if( (server_address.sin6_addr.s6_addr[0] & 0xFF) == 0xFE &&
        (server_address.sin6_addr.s6_addr[1] & 0xC0) == 0x80 ) {
        server_address.sin6_scope_id = 2;  // Interface index. TODO: Look this up.
    }

    // Connect to the server.
    if( connect( socket_handle, (struct sockaddr *) &server_address, sizeof(server_address) ) < 0) {
        perror( "Unable to connect to server" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    // Keep reading until we've read it all.
    while( ( count = read( socket_handle, buffer, BUFFER_SIZE - 1 ) ) > 0 ) {
        buffer[count] = '\0';
        fputs( buffer, stdout );
    }

    // Did the loop end above due to some error?
    if( count < 0 ) {
        perror( "Problem reading socket" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    // Close the socket to clean up.
    close( socket_handle );
    return EXIT_SUCCESS;
}
