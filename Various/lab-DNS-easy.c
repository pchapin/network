/*****************************************************************************
FILE          : dns-easy.c
PROGRAMMER    : Peter Chapin
LAST REVISION : 2003-02-19

(C) Copyright 2003 by Peter Chapin

This program does DNS lookups using the gethostbyname() helper function.

Please send comments or bug reports to

     Peter Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
*****************************************************************************/

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// ============
// Main Program
// ============

int main(int argc, char **argv)
{
  struct hostent *host_information;
         char     buffer[128];

  if (argc != 2) {
    fprintf(stderr, "%s: expected a hostname on the command line.\n", argv[0]);
    return 1;
  }

  // Look up the host information.
  host_information = gethostbyname(argv[1]);
  if (host_information == NULL) {
    fprintf(stderr, "%s: unable to lookup %s\n", argv[0], argv[1]);
    return 1;
  }

  // Convert the raw address into a human readible form. Display it.
  inet_ntop(AF_INET, host_information->h_addr_list[0], buffer, 128);
  printf("%s\n", buffer);

  return 0;
}
