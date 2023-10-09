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

// VTP segments have a four octet header and up to 1024 octets of data. This array has to hold
// the IP header as well (20 octets) since the entire IP datagram is returned through the raw
// socket.
unsigned char workspace[20 + 4 + 1024];

#define ADDRESS_BUFFER_SIZE 16  // Big enough for xxx.xxx.xxx.xxx (with null terminator)
char address_buffer[ADDRESS_BUFFER_SIZE];

int main( int argc, char **argv )
{
    int       socket_handle;
    socklen_t address_length;
    ssize_t   recv_count;
    struct sockaddr_in client_address;

    if( ( socket_handle = socket( PF_INET, SOCK_RAW, IPPROTO_VTP ) ) == -1 ) {
        perror( "Unable to create raw socket" );
        return EXIT_FAILURE;
    }

    while( 1 ) {
        address_length = sizeof(client_address);
        if( (recv_count = recvfrom( socket_handle,
                                    workspace,
                                    sizeof(workspace),
                                    0,
                                    (struct sockaddr *)&client_address,
                                    &address_length ) ) == -1 ) {
            perror( "Receive of VTP packet failed" );
        }
        else {
            inet_ntop( AF_INET, &client_address.sin_addr, address_buffer, ADDRESS_BUFFER_SIZE );
            printf( "VTP packet received from: %s\n", address_buffer );

            // Display everything.
            for( int i = 0; i < recv_count; ++i ) {
                printf("%02X ", workspace[i] );
            }
            printf( "\n" );
        }
    }

    close( socket_handle );
    return EXIT_SUCCESS;
}
