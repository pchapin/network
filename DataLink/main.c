
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
unsigned char buffer[BUFFER_SIZE];

void display_frame( unsigned char *frame, ssize_t size )
{
    const int WORKSPACE_SIZE = 48;      // 16 octets * 3 characters per octet.
    char workspace[WORKSPACE_SIZE + 1]; // Extra character for terminating null.
    char text[WORKSPACE_SIZE/2 + 1];    // Extra character for terminating null.
    int  position;
    int  text_position;

    workspace[WORKSPACE_SIZE] = '\0';   // Be sure this array stays null terminated.
    text[WORKSPACE_SIZE/2] = '\0';      // Be sure this array stays null terminated.
    memset( workspace, ' ', WORKSPACE_SIZE );
    memset( text, ' ', WORKSPACE_SIZE/2 );
    position = 0;
    text_position = 0;
    for( int i = 0; i < size; ++i ) {
        sprintf( &workspace[position], "%02X", frame[i] );
        workspace[position + 2] = ' ';  // Undo the null character added by sprintf.
        text[text_position] = isprint( frame[i] ) ? frame[i] : '.';
        if( position == 45 ) {
            // Output the workspace
            printf( "%s    %s\n", workspace, text );
            memset( workspace, ' ', WORKSPACE_SIZE );
            memset( text, ' ', WORKSPACE_SIZE/2 );
            position = 0;
            text_position = 0;
        }
        else {
            position += 3;
            text_position += 1;
        }
    }

    // Print a final (partially filled) workspace if necessary.
    if( position != 0 ) {
        printf( "%s    %s\n", workspace, text );
    }
}


int main( void )
{
    int socket_handle;
    struct sockaddr_ll address;

    // Create a socket for link layer use.
    if( ( socket_handle = socket( PF_PACKET, SOCK_RAW, htons(ETH_P_ALL) ) ) == -1 ) {
        perror( "Unable to create data link socket" );
        return EXIT_FAILURE;
    }

    // Bind it to a specific interface using the interface index.
    memset( &address, 0, sizeof(address) );
    address.sll_family = AF_PACKET;
    address.sll_protocol = htons(ETH_P_ALL);
    address.sll_ifindex = 2;  // Use "ip link show" to find out this value.
                              // TODO: look this up programmatically.
    if( bind( socket_handle, (struct sockaddr *)&address, sizeof(address) ) == -1 ) {
        perror( "Unable to bind interface to socket" );
        close( socket_handle );
        return EXIT_FAILURE;
    }

    while( 1 ) {
        ssize_t count;
        socklen_t address_length = sizeof(address);
        if( ( count = recvfrom( socket_handle,
                                buffer,
                                BUFFER_SIZE,
                                0,
                                (struct sockaddr *)&address,
                                &address_length ) ) == -1 ) {
            perror( "Problem receiving frame" );
        }

        printf( "\nReceived frame from: " );
        for( int i = 0; i < address.sll_halen; ++i ) {
            printf( "%02X", address.sll_addr[i] );
            if( i != address.sll_halen - 1 ) {
                printf( ":" );
            }
        }
        printf( "\n" );

        // Display everything.
        display_frame( buffer, count );
    }

    close( socket_handle );
    return EXIT_SUCCESS;
}
