
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 128

int main( int argc, char **argv )
{
    int error;
    struct addrinfo *result;
    struct addrinfo *current;
    char buffer[BUFFER_SIZE];

    // Check command line.
    if( argc != 3 ) {
        fprintf( stderr, "Usage: %s hostname service\n", argv[0] );
        return EXIT_FAILURE;
    }

    // Look up the address information.
    // Use the 'hints' parameter (NULL here) to filter the results for AF_INET or AF_INET6.
    if( ( error = getaddrinfo( argv[1], argv[2], NULL, &result ) ) != 0 ) {
        fprintf( stderr, "Unable to look up address information: %s\n", gai_strerror( error ) );
        return EXIT_FAILURE;
    }

    // Print out the addresses we found.
    int i = 1;
    current = result;
    while( current != NULL ) {
        void *address =
            (current->ai_family == AF_INET) ?
             (void *)&((struct sockaddr_in  *)current->ai_addr)->sin_addr :
             (void *)&((struct sockaddr_in6 *)current->ai_addr)->sin6_addr;
        inet_ntop( current->ai_family, address, buffer, BUFFER_SIZE );

        unsigned short port =
            (current->ai_family == AF_INET) ?
             ((struct sockaddr_in  *)current->ai_addr)->sin_port :
             ((struct sockaddr_in6 *)current->ai_addr)->sin6_port;
        port = ntohs( port );

        printf( "%2d. %s (port %u)\n", i++, buffer, port );
        current = current->ai_next;
    }

    // Release the linked list of addrinfo structures.
    freeaddrinfo( result );
    
    return EXIT_SUCCESS;
}
