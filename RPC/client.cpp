/*****************************************************************************
FILE   : client.cpp
AUTHOR : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>

This file contains a program that demonstrates calling remote procedures.
*****************************************************************************/

#include "extended_ints.h"

int main( int argc, char **argv )
{
    CLIENT        *client;
    two_intvalues  in;
    int_value     *outp;

    // Make sure we have the arguments we need.
    if( argc != 4 ) {
        fprintf( stderr, "usage: %s <hostname> <integer> <integer>\n", argv[0] );
        return 1;
    }

    // Create a client session with the server.
    client = clnt_create( argv[1], EXTENDED_INTS, EXTENDED_INTS_VER, "tcp" );
    if( client == NULL ) {
        fprintf( stderr, "Failed to connect to server!\n" );
        return 1;
    }

    // Prepare the arguments.
    in.arg1.type_flag = EXTENDED;
    in.arg1.int_value_u.value_e.extended_int_len = strlen( argv[2] ) + 1;
    in.arg1.int_value_u.value_e.extended_int_val = new char[strlen( argv[2] ) + 1];
    strcpy( in.arg1.int_value_u.value_e.extended_int_val, argv[2] );
    in.arg2.type_flag = EXTENDED;
    in.arg2.int_value_u.value_e.extended_int_len = strlen( argv[3] ) + 1;
    in.arg2.int_value_u.value_e.extended_int_val = new char[strlen( argv[3] ) + 1];
    strcpy( in.arg2.int_value_u.value_e.extended_int_val, argv[3] );
    
    // Add.
    if( ( outp = iadd_1( &in, client ) ) == NULL ) {
        clnt_perror( client, "Unable to call iadd_1" );
        return 1;
    }
    printf( "%s + %s = %s\n", argv[2], argv[3], outp->int_value_u.value_e.extended_int_val );
  
    // Multiply.
    if( ( outp = imul_1( &in, client ) ) == NULL ) {
        clnt_perror( client, "Unable to call imul_1" );
        return 1;
    }
    printf( "%s * %s = %s\n", argv[2], argv[3], outp->int_value_u.value_e.extended_int_val );

    return 0;
}
