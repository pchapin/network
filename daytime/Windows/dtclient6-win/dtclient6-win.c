/*****************************************************************************
FILE          : dtclient6-win.c
DESCRIPTION   : A simple IPv6 client program.
PROGRAMMER    : (C) Copyright 2010 by Peter C. Chapin <PChapin@vtc.vsc.edu>

This is a Winsock version of a simple daytime client program. It illustrates
how to write a simple client using Winsock. This version uses IPv6 (only).
*****************************************************************************/

#include <stdio.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define BUFFER_SIZE 128

// The following function displays an error message based on the
// error code returned by GetLastError().
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
  char                buffer[BUFFER_SIZE+1];
  const char         *port = "13";

  WSADATA             winsock_information;
  struct addrinfo     address_hints;
  struct addrinfo    *address_info;

  // Fill in the hints structure for later.
  memset(&address_hints, 0, sizeof(struct addrinfo));
  address_hints.ai_flags    = AI_NUMERICHOST;
  address_hints.ai_family   = AF_INET6;
  address_hints.ai_socktype = SOCK_STREAM;
  address_hints.ai_protocol = IPPROTO_TCP;

  // Do we have a command line argument?
  if (argc < 2) {
    fprintf(stderr, "Usage: %s IPv6Address [port]\n", argv[0]);
    return 1;
  }

  // Do we have an explicit port number?
  if (argc == 3) {
    port = argv[2];
  }

  // Try to intialize Winsock.
  if (WSAStartup(MAKEWORD(2, 2), &winsock_information) != 0) {
    fprintf(stderr, "Can't find a suitable Winsock\n");
    return 1;
  }
  if (!(LOBYTE(winsock_information.wVersion) == 2 &&
        HIBYTE(winsock_information.wVersion) == 2)) {
    fprintf(stderr, "Can't find a suitable Winsock\n");
    WSACleanup();
    return 1;
  }

  // Look up address information.
  if (getaddrinfo(argv[1], port, &address_hints, &address_info) != 0) {
    ErrorMessage();
    WSACleanup();
    return 1;
  }

  // Create an appropriate socket.
  if ((socket_handle = socket(address_info->ai_family,
                              address_info->ai_socktype,
                              address_info->ai_protocol)) == INVALID_SOCKET) {
    ErrorMessage();
    WSACleanup();
    return 1;
  }

  // Connect to the server.
  if (connect(socket_handle, address_info->ai_addr, address_info->ai_addrlen) == SOCKET_ERROR) {
    ErrorMessage();
    WSACleanup();
    return 1;
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
