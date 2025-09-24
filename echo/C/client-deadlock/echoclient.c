/*!
 * \file echoclient.c
 * \author Peter Chapin
 * \brief Implementation of an echo client in C.
 *
 * This version illustrates the deadlock problem that can arise.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Adjust the buffer size to explore when deadlock starts happening.
#define BUFFER_SIZE 1000000

static char buffer[BUFFER_SIZE];

int main( int argc, char **argv )
{
    int                 socket_handle;
    ssize_t             count;
    struct sockaddr_in  server_address;
    unsigned short      port = 7;
    int                 chunk_count;
    int                 byte_count;

    // Do we have a command line argument?
    if( argc < 2 ) {
        fprintf(stderr, "usage: %s IPAddress [port]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Do we have an explicit port number? If so, override the default.
    // TODO: Add error checking on the port number.
    if( argc == 3 ) {
        port = (unsigned short)atoi( argv[2] );
    }

    // Create a socket.
    if( ( socket_handle = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
        perror( "Unable to create socket" );
        return EXIT_FAILURE;
    }

    // Set up the server address.
    memset( &server_address, 0, sizeof( server_address ) );
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(port);
    if( inet_pton( AF_INET, argv[1], &server_address.sin_addr ) <= 0 ) {
        fprintf( stderr, "Unable to convert address into binary form.\n" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    // Connect to the server.
    if( connect( socket_handle, (struct sockaddr *)&server_address, sizeof( server_address ) ) == -1 ) {
        perror( "Unable to connect to server" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    // Attempt to write a huge array.
    memset( buffer, 'x', BUFFER_SIZE );
    write( socket_handle, buffer, BUFFER_SIZE );
    printf( "Wrote %d bytes to server.\n", BUFFER_SIZE );

    chunk_count = 0;
    byte_count  = 0;
    while( ( count = read( socket_handle, buffer, 1024 ) ) > 0 ) {
        buffer[count] = '\0';
        fputs( buffer, stdout );
        chunk_count++;
        byte_count += count;
        printf( "\nChunk: #%d (bytes so far: %d)\n", chunk_count, byte_count );

        if( byte_count == BUFFER_SIZE ) break;
    }

    // Did the loop end above due to some error?
    if( count == -1 ) {
        perror( "Problem reading socket" );
    }

    // Close the socket to clean up.
    close( socket_handle );
    printf( "\n" );
    return EXIT_SUCCESS;
}
