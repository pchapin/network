/****************************************************************************
FILE          : squareroot.c
LAST REVISION : October 5, 2000
SUBJECT       : The square root server.
PROGRAMMER    : Peter Chapin

This program contains a bare bones TCP/IP based server using the C
sockets API to do its work. It computes the square root of whatever
value it is given.

Please send comments or bug reports to

     Peter Chapin
     P.O. Box 317
     Randolph Center, VT 05061
     pchapin@vtc.vsc.edu
****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Service_Connection
 *
 * This function defines what this server does. For each incoming connection,
 * this function gets called. The address of the client is in the structure
 * pointed at by 'From.'
 *
 */
void Service_Connection(int Socket_Descriptor, struct sockaddr_in *From)
{
  char  *Greeting = "square-roots-r-us: v1.0\n";
  double Number;

  /* Send the greeting. */
  write(Socket_Descriptor, Greeting, strlen(Greeting));

  /* Keep looping until the other side closes the connection. */
  while (1) {
    char Buffer[256];
    int  Count = read(Socket_Descriptor, Buffer, 256);
      /* Read something. */

    if (Count == 0) break;

    /* And write the answer back. */
    sscanf(Buffer, "%lf", &Number);
    sprintf(Buffer, "%f\n", sqrt(Number));
    write(Socket_Descriptor, Buffer, strlen(Buffer));
  }
}


/*
 * Main Program
 *
 */
int main(int Argc, char **Argv)
{
  struct servent    *Service_Info;
  struct sockaddr_in Socket_Address;
         int         Socket_Descriptor;

  /* When a child dies, don't let it be a zombie. */
  signal(SIGCHLD, SIG_IGN);

  /* Get the port address of the silly service. */
  Service_Info = getservbyname("silly", "tcp");
  if (Service_Info == NULL) {
    fprintf(stderr,
      "%s: Can't locate service \"silly\" in services file.\n", Argv[0]
    );
    exit(1);
  }

  /* Create the socket. */
  Socket_Descriptor = socket(AF_INET, SOCK_STREAM, 0);

  /* Prepare the address. */
  memset(&Socket_Address, 0, sizeof(Socket_Address));
  /* Socket_Address.sin_port = Service_Info->s_port; */
  Socket_Address.sin_port = htons(9000);
  bind(
    Socket_Descriptor,
    (struct sockaddr *)&Socket_Address,
    sizeof(Socket_Address)
  );

  /* Listen on this socket. */
  listen(Socket_Descriptor, 5);

  /* Keep accepting connections. */
  while (1) {
    struct sockaddr_in From;
           int         New_Socket;
           int         Length = sizeof(struct sockaddr_in);

    New_Socket = accept(Socket_Descriptor, (struct sockaddr *)&From, &Length);
    if (New_Socket < 0) {
      /* Error during accept! Probably a signal. */
      continue;
    }
    
    if (fork() == 0) {

      /* In the child process. Service the connection. */
      close(Socket_Descriptor);
      Service_Connection(New_Socket, &From);
      close(New_Socket);
      exit(0);
    }

    /* In the parent process. Close the socket and loop back. */
    close(New_Socket);
  }
  
  return 0;
}
