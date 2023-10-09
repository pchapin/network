/****************************************************************************
FILE          : sendfile.c
LAST REVISION : October 1997
SUBJECT       : Program to send a file via IPX
PROGRAMMER    : Peter Chapin

This program demonstrates how to use IPX to send a file from one station
to another. It uses the NetWare SDK for IPX support. This program
demonstrates how to set up the IPX header, event control block, and
other details that are necessary to actually get packets out the
machine. This program sends only one byte from the file in each packet.
This is horribly inefficient.

Please send comments or bug reports to

     Peter Chapin
     P.O. Box 317
     Randolph Center, VT 05061
     pchapin@vtc.vsc.edu
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <nwipxspx.h>

/*
 * main()
 *
 * The main function. This function does some setup and then goes into a
 * loop where it reads the file and sends each byte to the destination
 * station one at a time.
 *
 */
int main(int argc, char **argv)
  {
    FILE *input;             // The file we are sending.
    int   ch;                // A character from the file.
    long  counter = 0;       // Counts the number of characters sent.

    IPXAddress  dest;        // We will hard code the destination addr.
    IPXHeader   header;      // This is the same for all outgoing packets.
    char        data;        // The data portion of the packet.
    BYTE        socket_number[2]; // The source socket number.
    ECB         event_block;      // Each IPX event needs an ECB.
    int         transport_time;   // I don't care about this, really.

    // Make sure a name exists on the command line.
    if (argc != 2) {
      printf("Usage: SENDFILE filename\n");
      return 1;
    }
    printf("\n");

    // Make sure IPX is there and ready to go.
    if (IPXInitialize() != 0x0000) {
      printf("Error: Can't initialize IPX.\n");
      return 1;
    }
    printf("Initialized IPX successfully.\n");

    // Make sure we can open the file.
    if ((input = fopen(argv[1], "rb")) == NULL) {
      printf("Error: Can't open %s for reading.\n", argv[1]);
      return 1;
    }
    printf("File \"%s\" opened successfully.\n\n", argv[1]);

    // Initialize the destination address. In a "real" application we
    // would have to use some clever way of finding out the address of
    // the destination station. For now we will just hard code it.

    memset(&dest, 0, sizeof(dest));
    dest.network[0] = 0x00; // Network 0x00000020 is the cable in G-111D.
    dest.network[1] = 0x00;
    dest.network[2] = 0x00;
    dest.network[3] = 0x20;
    dest.node[0]    = 0x00; // The node address of one of the stations in
    dest.node[1]    = 0x00; // G-111D.
    dest.node[2]    = 0x1B;
    dest.node[3]    = 0x33;
    dest.node[4]    = 0x9B;
    dest.node[5]    = 0x95;
    dest.socket[0]  = 0x80; // Socket number 0x8000 reserved for use
    dest.socket[1]  = 0x00; // by programs for NetWare.

    // Initialize the IPX header.
    memset(&header, 0, sizeof(header));
    header.length      = (sizeof(header) + 1) << 8; // Trick to get hi-lo.
    header.packetType  = 4;                         // Type 4 is "IPX."
    header.destination = dest;

    // Set the source socket number in hi-lo format. We don't need to
    // open the source socket, but we will still need one on the
    // outgoing packet.

    socket_number[0] = 0x80;
    socket_number[1] = 0x00;

    // Initialize the Event Control Block.
    memset(&event_block, 0, sizeof(event_block));
    event_block.socketNumber  = *(WORD *)socket_number;
    IPXGetLocalTarget(
      (BYTE *)&dest, event_block.immediateAddress, &transport_time
    );
    event_block.fragmentCount = 2;
    event_block.fragmentDescriptor[0].address = &header;
    event_block.fragmentDescriptor[0].size    = sizeof(header);
    event_block.fragmentDescriptor[1].address = &data;
    event_block.fragmentDescriptor[1].size    = 1;


    // Send one character at a time. It would be better to send blocks
    // of 546 characters at once. Actually 512 might be better when it
    // comes to doing file I/O efficiently.

    while ((ch = getc(input)) != EOF) {

      // Prepare the data part.
      data = (char)ch;

      IPXSendPacket(&event_block);
      while (event_block.inUseFlag == 0xFF) IPXRelinquishControl();
        // I guess we'll assume that it worked!

      // Keep the user entertained/informed.
      counter++;
      if (counter % 1024 == 0) printf("\r%ld bytes sent...", counter);
    }

    // Now send a null character to indicate EOF.
    data = 0;
    IPXSendPacket(&event_block);
    while (event_block.inUseFlag == 0xFF) IPXRelinquishControl();

    printf("\r%ld total bytes sent.\n", counter);

    // Clean up.
    IPXDisconnectFromTarget((BYTE *)&dest);
    fclose(input);

    return 0;
  }

