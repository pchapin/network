/*! \file    dns.c
    \brief   Simple DNS client for Unix that does name to IP address resolution.
    \author  Peter C. Chapin
    \date    2009-04-19
    \warning This code has not been formally tested!

This file contains a program that does DNS lookups "raw". That is, it does not use the
gethostbyname function. Instead it formats its own queries and sends the query as a UDP packet.
It then listens for a response, timing out if necessary. This program is intended to demonstrate
not only the DNS protocol but also basic UDP programming.

This code assumes that short is 16 bits and int is 32 bits. A better version would use uint16_t
and uint32_t for this purpose. This code also makes some assumptions about structure alignment
and padding.
*/

#include <stdio.h>

#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 128

//
// The data in this structure is stored in network byte order. Note that I assume there are no
// padding bytes between the members.
//
struct message_header {
    unsigned short id;
    unsigned short flags;
    unsigned short question_count;
    unsigned short answerRR_count;
    unsigned short authorityRR_count;
    unsigned short additionalRR_count;
};

//
// The following function is my handler for SIGALRM. All it needs to do is interrupt the
// recvfrom() system call.
//
void SIGALRM_handler(int signal_number)
{
    return;
}


//
// The following function builds a DNS query message to ask for the A record of the given name.
// It returns a pointer to a block of memory containing the message. It also returns, through
// length, the number of bytes in the message. The caller should free() the memory pointed at by
// the returned value when it is no longer needed.
//
char *construct_query(const char *name, int *length)
{
    struct message_header *header;
           char           *message;
           char           *question;
           int             index;

    *length  = sizeof(struct message_header) + strlen(name) + 2 + 4;
    message  = (char *)malloc(*length);
    question = message + sizeof(struct message_header);

    // Fill in the header.
    header = (struct message_header *)message;
    header->id    = htons(0xCAFE);
    header->flags = htons(0x0100);  // QR = 0, AA = 0, TC = 0, RD = 1, RA = 0.
    header->question_count     = htons(1);
    header->answerRR_count     = 0;
    header->authorityRR_count  = 0;
    header->additionalRR_count = 0;

    // Now fill in the name.
    while (*name) {
        index = 1;
        while (*name && *name != '.') {
            question[index++] = *name++;
        }
        *question  = index - 1;
        question += index;
        if (*name) name++;
    }
    *question++ = '\0';

    // Set up the query type and the query class. Accessing this memory as an unsigned short
    // causes alignment problems in some cases on some systems (for example, Alpha). Better
    // would be to fill in these 16 bit fields in two 8 bit pieces.
    //
    *(unsigned short *)question = htons(1);  // 'A' type query.
    question += 2;
    *(unsigned short *)question = htons(1);  // Internet address family.

    return message;
}


//
// The following function sends the DNS query pointed at by message to the host specified by
// server. Note that this function does minimal error handling. That should be fixed.
//
void send_query(
    int handle, const char *server_name, const char *message, int length)
{
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(53);
    inet_pton(AF_INET, server_name, &server_address.sin_addr);

    sendto(handle, message, length, 0,
           (struct sockaddr *) &server_address, sizeof(server_address));
}


//
// The following function receives the response packet from the DNS. It returns 0 if successful.
// Otherwise it returns -1.
//
int recv_response(int socket_handle, char **response, int *response_length)
{
    struct sockaddr_in server_address;
           int         address_length = sizeof(struct sockaddr_in);

    // What is the biggest UDP datagram that the name server might send to us? That is probably
    // documented in an RFC and it should be looked up. I know that it is fairly conservative.
    // For now I'll just use 4k. (I believe the right answer is actually 512 bytes).
    // 
    *response = (char *)malloc(4096);

    // Receive the packet from the server with a 10 second time-out.
    alarm(10);
    *response_length = recvfrom(socket_handle, *response, 4096, 0,
        (struct sockaddr *) &server_address, &address_length);

    if (*response_length == -1) {
        if (errno == EINTR) {
            fprintf(stderr, "Timed out: no response from name server\n");
        }
        else {
            perror("bad receive");
        }
        return -1;
    }
    return 0;
}


//
// The following function displays the IP address in the answer section of the response packet.
//
void display_response(char *response, int response_length)
{
    struct message_header *header;
           char            buffer[BUFFER_SIZE];
           int             response_code;

    // Check the response code.
    header = (struct message_header *)response;
    response_code = ntohs(header->flags) & 0x000F;
    printf("Response received. Response code = %d\n", response_code);

    // Bag out if this is not a success code.
    if (response_code != 0) return;

    // Skip over the header.
    response += sizeof(struct message_header);

    // Skip over the question.
    while (*response++);
    response += 4;

    // Assume that the first answer is the right answer. Skip over the leading fields of the
    // answer. I have to deal with the possibility that the domain name is "compressed" as
    // according to RFC-1035.
    // 
    if ((*response & 0xC0) != 0) response += 2;
    else {
        while (*response++);
    }
    response += 10;

    // Decode the address.
    inet_ntop(AF_INET, response, buffer, BUFFER_SIZE);
    printf("A = %s\n", buffer);
}


// ============
// Main Program
// ============

int main(int argc, char **argv)
{
    char *query;            // Points at the query packet.
    char *response;         // Points at the response packet.
    int   query_length;     // The number of bytes in the query packet.
    int   response_length;  // The number of bytes in the response packet.
    int   socket_handle;

    struct sigaction myaction;

    // Do I have the command line arguments I need?
    if (argc != 3) {
        fprintf(stderr, "usage: %s server-IP name-to-query\n", argv[0]);
        return 1;
    }

    // Set up the signal handling.
    myaction.sa_handler = SIGALRM_handler;
    myaction.sa_flags   = 0;
    sigemptyset(&myaction.sa_mask);
    sigaction(SIGALRM, &myaction, NULL);

    // Create an appropriate socket.
    if ((socket_handle = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(argv[0]);
        return 1;
    }

    // Build the query packet.
    query = construct_query(argv[2], &query_length);

    // Send my query.
    send_query(socket_handle, argv[1], query, query_length);
    free(query);

    // Get the reply. Ignore a time out.
    if (recv_response(socket_handle, &response, &response_length) != -1) {
        display_response(response, response_length);
    }
    free(response);

    close(socket_handle);
    return 0;
}
