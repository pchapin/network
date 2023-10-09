/****************************************************************************
FILE   : dtserver.c
SUBJECT: Implementation of an iterative daytime server in C.

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
#include <time.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#ifndef S_SPLINT_S    // Workaround for splint.
#include <unistd.h>
#endif

#define BUFFER_SIZE 128

int main(int argc, char **argv)
{
    int                 listen_handle;        // Handle of accepting socket.
    int                 connection_handle;    // Handle representing a connection.
    struct sockaddr_in  server_address;       // Defines listening address.
    unsigned short      port = 13;            // Listening port number.
    struct sockaddr_in  client_address;       // Stores address of client.
    socklen_t           client_length;        // Size of client_address structure.
    char                buffer [BUFFER_SIZE]; // General purpose buffer for data.
    char                control[BUFFER_SIZE]; // Buffer for control information.
    size_t              buffer_length;        // Number of characters in buffer.
    time_t              ticks;                // Current time.

    struct iovec buffer_vector = {
        .iov_base = buffer,
        .iov_len  = BUFFER_SIZE         // Change before sending to the correct length.
    };

    struct msghdr message_header = {
        .msg_name       = NULL,         // Don't care about client address.
        .msg_namelen    = 0,
        .msg_iov        = &buffer_vector,
        .msg_iovlen     = 1,
        .msg_control    = control,
        .msg_controllen = 0,            // No control information.
        .msg_flags      = 0             // Only meaningful for recvmsg().
    };

    // Do we have an explicit port address? If so, override the default.
    if (argc == 2) {
        port = atoi(argv[1]);
    }

    // Create the server socket as an IPv4 TCP socket.
    if ((listen_handle = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
        perror("Unable to create socket");
        return 1;
    }

    // Prepare the server socket address structure.
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port        = htons(port);

    // Bind the server socket.
    if (bind(listen_handle, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Unable to bind socket");
        close(listen_handle);
        return 1;
    }

    // Allow incoming connections.
    if (listen(listen_handle, 32) < 0) {
        perror("Unable to listen");
        close(listen_handle);
        return 1;
    }

    // Accept connections forever.
    while (1) {
        memset(&client_address, 0, sizeof(client_address));
        client_length = sizeof(client_address);

        // Block until a client comes along.
        connection_handle =
            accept(listen_handle, (struct sockaddr *) &client_address, &client_length);

        // Check for errors from accept.
        if (connection_handle < 0) {
            perror("Accept failed");
        }
        else {
            // Display informational message.
            inet_ntop(AF_INET, &client_address.sin_addr, buffer, BUFFER_SIZE);
            printf("Accepted client connection from: %s\n", buffer);

            // Perform the requested service. (Send the current time of day)
            ticks = time(NULL);
            snprintf(buffer, BUFFER_SIZE, "%.24s\r\n", ctime(&ticks));
            buffer_length = strlen(buffer);
            buffer_vector.iov_len = buffer_length;
            if (sendmsg(connection_handle, &message_header, 0) != buffer_length) {
                perror("Send failed");
            }

            // Close this connection.
            close(connection_handle);
        }
    }
}
