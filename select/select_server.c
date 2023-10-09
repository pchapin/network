/*!
 * \file select_server.c
 * \author Peter C. Chapin
 * \brief Program to demonstrate select().
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#ifndef S_SPLIT_S     // Workaround for splint.
#include <unistd.h>
#endif

#define BUFFER_SIZE 128

// ============
// Main Program
// ============

int max( int x, int y )
{
    return ( x > y ) ? x : y;
}

int main( int argc, char **argv )
{
    int socket_handle1;
    int socket_handle2;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_length;
    char buffer[BUFFER_SIZE];

    // Create the sockets.
    if( ( socket_handle1 = socket( PF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
        perror( "Unable to create socket" );
        return 1;
    }
    if( ( socket_handle2 = socket( PF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
        perror( "Unable to create socket" );
        close( socket_handle1 );
        return 1;
    }

    // First fill in the server address structure.
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl( INADDR_ANY );

    // Bind the sockets.
    server_address.sin_port = htons( 9000 );
    if( bind( socket_handle1,
              (const struct sockaddr *)&server_address, sizeof(server_address) ) == -1 ) {
        perror( "Unable to bind to port 9000" );
        close( socket_handle1 );
        close( socket_handle2 );
        return 1;
    }
    server_address.sin_port = htons( 9001 );
    if( bind( socket_handle2,
              (const struct sockaddr *)&server_address, sizeof(server_address) ) == -1 ) {
        perror( "Unable to bind to port 9001" );
        close( socket_handle1 );
        close( socket_handle2 );
        return 1;
    }

    while( 1 ) {
        struct timeval timeout_interval;
        fd_set read_handles;
        int    select_result;

        timeout_interval.tv_sec  = 10;
        timeout_interval.tv_usec = 0;
        FD_ZERO( &read_handles );
        FD_SET( socket_handle1, &read_handles );
        FD_SET( socket_handle2, &read_handles );

        select_result = select( max( socket_handle1, socket_handle2 ) + 1,
                                &read_handles,
                                NULL,
                                NULL,
                                &timeout_interval );

        if( select_result == -1 ) {
            perror( "select() failed" );
        }
        else if( select_result == 0 ) {
            printf( "Timed out!\n" );
            break;
        }
        else {
            if( FD_ISSET( socket_handle1, &read_handles ) ) {
                client_length = sizeof(client_address);
                recvfrom( socket_handle1,
                          buffer,
                          BUFFER_SIZE,
                          0,
                          (struct sockaddr *)&client_address, &client_length );
                printf( "Port 9000: %s\n", buffer );
            }

            if( FD_ISSET( socket_handle2, &read_handles ) ) {
                client_length = sizeof(client_address);
                recvfrom( socket_handle2,
                          buffer,
                          BUFFER_SIZE,
                          0,
                          (struct sockaddr *)&client_address, &client_length );
                printf( "Port 9001: %s\n", buffer );
            }
        }
    }

    close( socket_handle1 );
    close( socket_handle2 );

    return EXIT_SUCCESS;
}

