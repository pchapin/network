/****************************************************************************
FILE          : recvfile.c
LAST REVISION : April 1997
SUBJECT       : Program to receive a file via IPX
PROGRAMMER    : Peter Chapin

This program is the complement of sendfile.c.

Please send comments or bug reports to

     Peter Chapin
     P.O. Box 317
     Randolph Center, VT 05061
     pchapin@vtc.vsc.edu
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <nwipxspx.h>

int recv(BYTE *socket_number);
  // This function gets a single character from the network.


/*
 * main()
 *
 * The main function.
 *
 */
int main(int argc, char **argv)
  {
    FILE *output;            // The file we are getting.
    BYTE  socket_number[2];  // The socket number we are listening to.
    int   ch;                // A character for the file.

    // Make sure a name exists on the command line.
    if (argc != 2) {
      printf("Usage: RECVFILE filename\n");
      return 1;
    }

    // Make sure IPX is there and ready to go.
    if (IPXInitialize() != 0x0000) {
      printf("Error: Can't initialize IPX.\n");
      return 1;
    }
    printf("Initialized IPX successfully.\n");

    // Make sure we can open the file.
    if ((output = fopen(argv[1], "wb")) == NULL) {
      printf("Error: Can't open %s for reading.\n", argv[1]);
      return 1;
    }

    // Open a socket for listening. This will be socket 0x8000. Notice
    // that we have to put the high byte in the first array element.
    socket_number[0] = 0x80;
    socket_number[1] = 0x00;

    if (IPXOpenSocket(socket_number, 0) != 0x0000) {
      printf("Error: Can't open an IPX socket.\n");
      fclose(output);
      return 1;
    }
    printf("Opened IPX socket successfully.\n\n");

    // Get one character at a time. Is this a good idea?
    while ((ch = recv(socket_number)) != EOF) {
      putc(ch, output);
    }

    // Clean up.
    IPXCloseSocket(*(WORD *)socket_number);
    fclose(output);

    return 0;
  }


/*
 * recv()
 *
 * This function does the grunt work of getting a single character.
 *
 */
int recv(BYTE *socket_number)
  {
    ECB        event_block;    // Each IPX event needs an ECB.
    IPXHeader  header;         // The header is one fragment.
    char       data[546];      // Allocate max size just in case.

    // Initialize the Event Control Block.
    memset(&event_block, 0, sizeof(event_block));
    event_block.socketNumber  = *(WORD *)socket_number;
    event_block.fragmentCount = 2;
    event_block.fragmentDescriptor[0].address = &header;
    event_block.fragmentDescriptor[0].size    = sizeof(header);
    event_block.fragmentDescriptor[1].address = &data;
    event_block.fragmentDescriptor[1].size    = 546;

    // Wait for a packet.
    IPXListenForPacket(&event_block);
    while (event_block.inUseFlag != 0xFE) IPXRelinquishControl();

    // I guess we'll assume that it worked! We only care about the first
    // byte of the data. Supposedly we will only be getting single byte
    // packets (data portion) from the sending application.

    return data[0];
  }
