/****************************************************************************
FILE   : dtclient.c
SUBJECT: Implementation of a simple daytime client in C.

Please send comments or bug reports to

     Peter C. Chapin
     Computer Information Systems
     Vermont Technical College
     Williston, VT 05495
     pchapin@vtc.edu
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#ifndef S_SPLINT_S    // Workaround for splint.
#include <unistd.h>
#endif

#define BUFFER_SIZE 128

int main( int argc, char **argv )
{
    int                 socket_handle;
    int                 count;
    char                buffer[BUFFER_SIZE];
    struct sockaddr_in  server_address;
    unsigned short      port = 13;

    // Do we have a command line argument?
    if( argc < 2 ) {
        fprintf( stderr, "usage: %s IPAddress [port]\n", argv[0] );
        return EXIT_FAILURE;
    }

    // Do we have an explicit port number? If so, override the default.
    if( argc == 3 ) {
        port = atoi( argv[2] );
    }

    // Create a socket.
    if( ( socket_handle = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
        perror( "Unable to create socket" );
        return EXIT_FAILURE;
    }

    // Set up the server address.
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons( port );
    if( inet_pton( AF_INET, argv[1], &server_address.sin_addr ) <= 0 ) {
        fprintf( stderr, "Unable to convert address into binary form.\n" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    // Connect to the server.
    if( connect( socket_handle, (struct sockaddr *)&server_address, sizeof(server_address) ) == -1 ) {
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
