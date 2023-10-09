/*****************************************************************************
FILE          : dtclient-win.c
DESCRIPTION   : A simple network client program.
PROGRAMMER    : (C) Copyright 2010 by Peter C. Chapin <PChapin@vtc.vsc.edu>

This is a Winsock version of a simple daytime client program. It illustrates
how to write a simple client using Winsock.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#define BUFFER_SIZE 128

// The following function displays an error message based on the error code returned by
// GetLastError().
//
void ErrorMessage(void)
{
  LPVOID message_buffer;
  DWORD error_code = GetLastError();

  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    error_code,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &message_buffer,
    0,
    NULL
  );

  fprintf(stderr, "Error %d: %s\n", error_code, message_buffer);
  LocalFree(message_buffer);
}


int main(int argc, char **argv)
{
  SOCKET              socket_handle;
  int                 count;
  char                buffer[BUFFER_SIZE];
  struct sockaddr_in  server_address;
  unsigned short      port = 13;

  WSADATA             winsock_information;

  // Do we have a command line argument?
  if (argc < 2) {
    fprintf(stderr, "Usage: %s IPAddress\n", argv[0]);
    return 1;
  }

  // Do we have an explicit port number?
  if (argc == 3) {
    port = atoi(argv[2]);
  }

  // Try to intialize Winsock.
  if (WSAStartup(MAKEWORD(2, 0), &winsock_information) != 0) {
    fprintf(stderr, "Can't find a suitable Winsock\n");
    return 0;
  }
  if (!(LOBYTE(winsock_information.wVersion) == 2 &&
        HIBYTE(winsock_information.wVersion) == 0)) {
    fprintf(stderr, "Can't find a suitable Winsock\n");
    WSACleanup();
    return 0;
  }

  // Create an appropriate socket.
  if ((socket_handle = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    ErrorMessage();
    WSACleanup();
    return 0;
  }

  // Prepare the address of the server.
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port   = htons(port);
  server_address.sin_addr.S_un.S_addr = inet_addr(argv[1]);

  // Connect to the server.
  if (connect(socket_handle, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
    ErrorMessage();
    WSACleanup();
    return 0;
  }

  // Keep reading until I've read it all.
  while ((count = recv(socket_handle, buffer, BUFFER_SIZE, 0)) > 0) {
    buffer[count] = '\0';
    fputs(buffer, stdout);
  }

  // Did the loop end above due to some error?
  if (count < 0) {
    ErrorMessage();
  }

  closesocket(socket_handle);
  WSACleanup();
  return 0;
}
