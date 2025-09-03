/*!
 * \file echoclient.c
 * \author Peter Chapin
 * \brief Implementation of an echo client in C.
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
    struct sockaddr_in  server_address;
    unsigned short      port = 7;

    // Do we have a command line argument?
    if( argc < 2 ) {
        fprintf( stderr, "usage: %s IPAddress [port]\n", argv[0] );
        return EXIT_FAILURE;
    }

    // Do we have an explicit port number? If so, override the default.
    // TODO: Add error handling to ensure the port number is valid.
    if( argc == 3 ) {
        port = (unsigned short) atoi( argv[2] );
    }

    // Create a socket.
    if( ( socket_handle = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
        perror( "Unable to create socket" );
        return EXIT_FAILURE;
    }

    // Set up the server address.
    memset( &server_address, 0, sizeof( server_address ) );
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons( port );
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

    // Keep reading user input and sending it to the server.
    printf( "> " );
    while( fgets( buffer, BUFFER_SIZE, stdin ) != NULL ) {

        // TODO: Deal with partial writes.
        write( socket_handle, buffer, strlen( buffer ) );

        while( ( count = read( socket_handle, buffer, BUFFER_SIZE - 1 ) ) > 0 ) {
            buffer[count] = '\0';
            fputs( buffer, stdout );

            // If this line contains a '\n' character, we are done for now.
            if( strchr( buffer, '\n' ) != NULL ) break;
        }

        // Did the loop end above due to some error?
        if( count == -1 ) {
            perror( "Problem reading socket" );
        }
        printf( "> " );
    }

    // Close the socket to clean up.
    close( socket_handle );
    printf( "\n" );
    return EXIT_SUCCESS;
}
