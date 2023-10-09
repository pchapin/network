/*****************************************************************************
FILE          : dtserver-win.c
DESCRIPTION   : A simple network server program.
PROGRAMMER    : (C) Copyright by Peter C. Chapin <PChapin@vtc.vsc.edu>

This is a Winsock version of a simple daytime server program. It illustrates
how to write a simple, iterative server using Winsock.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  SOCKET              listen_handle;
  SOCKET              connection_handle;
  struct sockaddr_in  server_address;
  struct sockaddr_in  client_address;
  int                 client_length;
  char                buffer[BUFFER_SIZE];
  time_t              ticks;
  unsigned short      port = 13;

  WSADATA             winsock_information;

  // Do we have an explicit port address?
  if (argc == 2) {
    port = atoi(argv[1]);
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

  // Create the server socket.
  if ((listen_handle = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    ErrorMessage();
    WSACleanup();
    return 0;
  }

  // Prepare the server socket address structure.
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family  = AF_INET;
  server_address.sin_port    = htons(port);
  server_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

  // Bind the server socket.
  if (bind(listen_handle, (struct sockaddr *) &server_address, sizeof(server_address)) == SOCKET_ERROR) {
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

    // Display informational message.
    printf("Accepted client connection from: %s\n", inet_ntoa(client_address.sin_addr));

    // Send the current time of day. Note: The Visual C library doesn't have snprintf.
    ticks = time(NULL);
    sprintf(buffer, "%.24s\r\n", ctime(&ticks));
    send(connection_handle, buffer, (int)strlen(buffer), 0);

    // Close this connection.
    closesocket(connection_handle);
  }
}
