/*****************************************************************************
FILE   : client.c
AUTHOR : (C) Copyright 2021 by Peter Chapin <pchapin@vtc.edu>

This file contains a program that demonstrates calling remote procedures.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file_services.h"

int main( int argc, char **argv )
{
    CLIENT     *client;
    bool_t     *open_result;
    readresult *read_result;

    // Make sure we have the arguments we need.
    if( argc != 3 ) {
        fprintf( stderr, "usage: %s <hostname> <filename>\n", argv[0] );
        return EXIT_FAILURE;
    }

    // Create a client session with the server.
    client = clnt_create( argv[1], FILE_SERVICES, FILE_SERVICES_VER, "tcp" );
    if( client == NULL ) {
        fprintf( stderr, "Failed to connect to server!\n" );
        return EXIT_FAILURE;
    }

    open_result = open_1( &argv[2], client );
    if( open_result == NULL ) {
        fprintf( stderr, "Open call failed!\n" );
        return EXIT_FAILURE;
    }
    if( *open_result == 0 ) {
        fprintf( stderr, "Unable to open file %s\n", argv[2] );
        return EXIT_FAILURE;
    }

    while( 1 ) {
        int request_count = 512;
        read_result = read_1( &request_count, client );
        if( read_result == NULL ) {
            fprintf( stderr, "Read call failed!\n" );
            return EXIT_FAILURE;
        }
        if( read_result->count == 0 ) break;

        write( 1, read_result->buffer, read_result->count );
    }

    close_1( NULL, client );
    clnt_destroy( client );
    return EXIT_SUCCESS;
}
