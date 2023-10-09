/*****************************************************************************
FILE   : file_services.x
AUTHOR : (C) Copyright 2021 by Peter Chapin <pchapin@vtc.edu>

This is a demonstration of Sun's ONC RPC that implements a simple file server.
*****************************************************************************/

/* A variable length string with a maximum size of 256. */
typedef string filename_t<256>;

/* A structure to hold the result of reading. Note that a count of zero means EOF. */
struct readresult {
  int    count;         /* The number of interesting bytes in the array below. */
  opaque buffer[512];   /* Raw data from the file. */
};

program FILE_SERVICES {
  version FILE_SERVICES_VER {
    bool       OPEN(filename_t) = 1; /* Opens file. Returns "true" on success. */
    readresult READ(int)        = 2; /* Reads the next chunk from the file. */
    int        CLOSE(void)      = 3; /* Closes the file. */
  } = 1;         /* Version number */
} = 0x30009000;  /* Program number */
