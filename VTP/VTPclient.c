/*
 * This program is a sandbox for VTP development.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "VTP.h"

// VTP segments have a four octet header and up to 1024 octets of data.
unsigned char workspace[4 + 1024];

int main( int argc, char **argv )
{
    int socket_handle;
    struct sockaddr_in server_address;

    // Do we have a command line argument?
    if( argc < 2 ) {
        fprintf( stderr, "usage: %s IPAddress\n", argv[0] );
        return EXIT_FAILURE;
    }

    if( ( socket_handle = socket( PF_INET, SOCK_RAW, IPPROTO_VTP ) ) == -1 ) {
        perror( "Unable to create raw socket" );
        return EXIT_FAILURE;
    }

    // Prepare a connection request segment.
    workspace[0] = 0;  // data_length == 0 (16 bits)
    workspace[1] = 0;
    workspace[2] = 0;  // flags (16 bits)
    workspace[3] = VTP_CONN_REQ;

    // Set up the server address.
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family = AF_INET;
    //server_address.sin_port = htons( port );
    if( inet_pton( AF_INET, argv[1], &server_address.sin_addr ) <= 0 ) {
        fprintf( stderr, "Unable to convert address into binary form.\n" );
        close( socket_handle );
        return EXIT_FAILURE;
    }
    
    if( sendto( socket_handle, workspace, 4, 0, (struct sockaddr *)&server_address, sizeof(server_address) ) == -1 ) {
        perror( "Unable to send segment" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    close( socket_handle );
    return EXIT_SUCCESS;
}
