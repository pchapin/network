/*!
 * \file select_client.c
 * \author Peter C. Chapin
 * \brief Program to exercise the select() demonstration server.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#ifndef S_SPLIT_S     // Workaround for splint.
#include <unistd.h>
#endif


// ============
// Main Program
// ============

int main( int argc, char **argv )
{
    int socket_handle;
    struct sockaddr_in server_address;
    char workspace[128];

    // Do I have a command line argument? I need at least the IP address.
    if( argc < 2 ) {
        fprintf( stderr, "Usage: %s IPAddress\n", argv[0] );
        return EXIT_FAILURE;
    }

    // Create a socket.
    if( ( socket_handle = socket( PF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
        perror( "Unable to create socket" );
        return 1;
    }

    // First fill in the server address structure.
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family = AF_INET;
    if( inet_pton( AF_INET, argv[1], &server_address.sin_addr ) <= 0 ) {
        fprintf( stderr, "Unable to convert address into binary form.\n" );
        close( socket_handle );
        return 1;
    }

    // TODO: Add error checking on calls to sendto().

    strcpy( workspace, "Sent to port 9000" );
    server_address.sin_port = htons( 9000 );
    sendto(
        socket_handle,
        workspace,
        strlen( workspace ) + 1,
        0,
        (const struct sockaddr *)&server_address,
        sizeof(server_address) );

    strcpy( workspace, "Sent to port 9001" );
    server_address.sin_port = htons( 9001 );
    sendto(
        socket_handle,
        workspace,
        strlen( workspace ) + 1,
        0,
        (const struct sockaddr *)&server_address,
        sizeof(server_address) );

    close( socket_handle );

    return EXIT_SUCCESS;
}
