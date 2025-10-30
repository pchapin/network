/**
 * FILE   : server.c
 * AUTHOR : (C) Copyright 2025 by Peter Chapin <pchapin@vermontstate.edu>
 *
 * This file contains the implementation of the remote procedures.
 */

#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "file_services.h"

int file_handle = -1;

// The following function removes ".." from the path.
static void cleanse_path( char *name )
{
    // IMPLEMENT ME!
}


bool_t *open_1_svc( filename_t *namep, struct svc_req *dummy )
{
    static bool_t result = 0;
    char relative_path[128];

    strcpy( relative_path, "root/" );
    strcat( relative_path, *namep );
    cleanse_path( relative_path );

    if( file_handle != -1 ) close( file_handle );
    file_handle = open( relative_path, O_RDONLY );
    if( file_handle == -1 ) return &result;

    result = 1;
    return &result;
}


readresult *read_1_svc( int *countp, struct svc_req *dummy )
{
    static readresult result;
    int requested_count = ( *countp <= 512 ) ? *countp : 512;

    if( file_handle == -1 ) {
        result.count = 0;
    }
    else {
        result.count = read( file_handle, result.buffer, requested_count );
    }
    return &result;
}


int *close_1_svc( void *dummy1, struct svc_req *dummy2 )
{
    static int result = 1;
    
    if( file_handle != -1 ) close( file_handle );
    return &result;
}
