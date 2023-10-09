/****************************************************************************
FILE   : dtclient.c
SUBJECT: Implementation of a simple daytime client in C.

This version uses the SCTP protocol.

Please send comments or bug reports to

     Peter C. Chapin
     Computer Information Systems
     Vermont Technical College
     Williston, VT 05495
     PChapin@vtc.vsc.edu
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

int main(int argc, char **argv)
{
    int                 socket_handle;
    char                buffer [BUFFER_SIZE];  // Buffer to hold data on the association.
    char                control[BUFFER_SIZE];  // Buffer to hold control information.
    int                 count;                 // The number of data bytes received.
    struct sockaddr_in  server_address;
    unsigned short      port = 13;

    // Structure describing the buffer for data.
    struct iovec buffer_vector = {
        .iov_base = buffer,
        .iov_len  = BUFFER_SIZE - 1     // Leave space for a null byte added locally.
    };

    // Message header structure.
    struct msghdr message_header = {
        .msg_name       = NULL,         // Don't care about server address.
        .msg_namelen    = 0,
        .msg_iov        = &buffer_vector,
        .msg_iovlen     = 1,
        .msg_control    = control,
        .msg_controllen = BUFFER_SIZE,  // Value/result. Be sure to reset after each call.
        .msg_flags      = 0             //               Be sure to reset after each call.
    };

    // Do we have a command line argument?
    if (argc < 2) {
        fprintf(stderr, "usage: %s IPAddress [port]\n", argv[0]);
        return 1;
    }

    // Do we have an explicit port number? If so, override the default.
    if (argc == 3) {
        port = atoi(argv[2]);
    }

    // Create a socket.
    if ((socket_handle = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
        perror("Unable to create socket");
        return 1;
    }

    // Set up the server address.
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(port);
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
        fprintf(stderr, "Unable to convert address into binary form.\n");
        close(socket_handle);
        return 1;
    }

    // Connect to the server.
    if (connect(socket_handle, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Unable to connect to server");
        close(socket_handle);
        return 1;
    }

    // Keep reading until we've read it all.
    while ((count = recvmsg(socket_handle, &message_header, 0)) > 0) {
        buffer[count] = '\0';
        fputs(buffer, stdout);
        if( message_header.msg_flags & MSG_TRUNC ) {
            printf( "\nWARNING: Data truncated!\n" );
        }
        message_header.msg_controllen = BUFFER_SIZE;
        message_header.msg_flags = 0;
    }

    // Did the loop end above due to some error?
    if (count < 0) {
        perror("Problem reading socket");
        close(socket_handle);
        return 1;
    }

    // Close the socket to clean up.
    close(socket_handle);
    return 0;
}
