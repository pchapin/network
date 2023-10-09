/*****************************************************************************
FILE          : dtserver6-win.c
DESCRIPTION   : A simple IPv6 server program.
PROGRAMMER    : (C) Copyright 2010 by Peter C. Chapin <PChapin@vtc.vsc.edu>

This is a Winsock version of a simple daytime server program. It illustrates
how to write a simple, iterative server using Winsock. This is for IPv6 (only).
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  SOCKET            listen_handle;
  SOCKET            connection_handle;
  SOCKADDR_STORAGE  client_address;
  int               client_length;
  char              buffer[BUFFER_SIZE];
  time_t            ticks;
  const char       *port = "13";

  WSADATA           winsock_information;
  struct addrinfo   address_hints;
  struct addrinfo  *address_info;

  // Fill in the hints structure for later.
  memset(&address_hints, 0, sizeof(struct addrinfo));
  address_hints.ai_flags    = AI_NUMERICHOST | AI_PASSIVE;
  address_hints.ai_family   = AF_INET6;
  address_hints.ai_socktype = SOCK_STREAM;
  address_hints.ai_protocol = IPPROTO_TCP;

  // Do we have a command line argument?
  if (argc < 2) {
    fprintf(stderr, "Usage: %s port\n", argv[0]);
    return 1;
  }

  // Do we have an explicit port address?
  if (argc == 2) {
    port = argv[1];
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

  // Look up address information.
  if (getaddrinfo(NULL, port, &address_hints, &address_info) != 0) {
    ErrorMessage();
    WSACleanup();
    return 0;
  }

  // Create the server socket.
  if ((listen_handle = socket(address_info->ai_family,
                              address_info->ai_socktype,
                              address_info->ai_protocol)) == INVALID_SOCKET) {
    ErrorMessage();
    WSACleanup();
    return 0;
  }

  // Bind the server socket.
  if (bind(listen_handle, address_info->ai_addr, address_info->ai_addrlen) == SOCKET_ERROR) {
    ErrorMessage();
    WSACleanup();
    return 0;
  }

  // Allow incoming connections.
  if (listen(listen_handle, 32) == SOCKET_ERROR) {
    ErrorMessage();
    WSACleanup();
    return 0;
  }

  // Accept connections forever.
  while (1) {
    memset(&client_address, 0, sizeof(client_address));
    client_length = sizeof(client_address);

    // Block until a client comes along.
    connection_handle = accept(listen_handle, (struct sockaddr *) &client_address, &client_length);

    // Check for errors from accept.
    if (connection_handle == INVALID_SOCKET) {
      ErrorMessage();
      WSACleanup();
      return 0;
    }

    // Send the current time of day. Note: The Visual C library doesn't have snprintf.
    ticks = time(NULL);
    sprintf(buffer, "%.24s\r\n", ctime(&ticks));
    send(connection_handle, buffer, (int)strlen(buffer), 0);

    // Close this connection.
    closesocket(connection_handle);
  }
}
