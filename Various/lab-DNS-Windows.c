/*! \file    dns.c
    \brief   Simple DNS client that does name to IP address resolution.
    \author  YOUR NAME
    \date    DATE
    \warning This code has not been formally tested!

This file contains a program that does DNS lookups "raw". That is, it does not use the
gethostbyname function. Instead it formats its own queries and sends the query as a UDP packet.
It then listens for a response, timing out if necessary. This program is intended to demonstrate
not only the DNS protocol but also basic UDP programming.

This code assumes that short is 16 bits and int is 32 bits. A better version would use uint16_t
and uint32_t for this purpose. This code also makes some assumptions about structure alignment
and padding.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//! Defines the structure of a DNS query header.
/*!
 *  The data in this structure is stored in network byte order. Note that we assume there are no
 *  padding bytes between the members and that 'short' is 16 bits.
 */
struct datagram_header {
  unsigned short id;
  unsigned short flags;
  unsigned short question_count;
  unsigned short answerRR_count;
  unsigned short authorityRR_count;
  unsigned short additionalRR_count;
};


//! Display an error message based on GetLastError()
/*!
 *  This function outputs a single line of text onto the standard error device containing the
 *  standard Windows message corresponding to the current error code (as returned by the
 *  GetLastError() function). Call this function after any Winsock function fails to output
 *  a specific error message to the user.
 */
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

  fprintf(stderr, "*** Windows Error %d: %s\n", error_code, message_buffer);
  LocalFree(message_buffer);
}


//! Initialize the Windows sockets library.
/*!
 *  This function prepares the Windows sockets library for use. Support for version 2.0 of the
 *  library is required. After this function returns successfully, the caller must eventually
 *  call WSACleanup() before program termination to undo the effects of this function.
 *  
 *  \return Zero on success, or -1 if Windows sockets failed to initalize or if support for the
 *  necessary version is not available.
 */
int initialize_winsock(void)
{
  WSADATA  winsock_information;

  if (WSAStartup(MAKEWORD(2, 0), &winsock_information) != 0) {
    return -1;
  }
  if (!(LOBYTE(winsock_information.wVersion) == 2 &&
        HIBYTE(winsock_information.wVersion) == 0)) {
    WSACleanup();
    return -1;
  }
  return 0;
}


//! Build a DNS query datagram.
/*!
 *  This function allocates space for and builds a DNS query given the desired name.
 *   
 *  \param name A pointer to a domain name to resolve.
 *  \param length A pointer to an integer that receives the total number of bytes in the
 *  constructed datagram (including the header).
 *   
 *  \return A pointer to the constructed datagram. This pointer must be freed once the datagram
 *  is no longer needed. The NULL pointer is returned if an out-of-memory error occurs. In that
 *  case the value of '*length' is undefined.
 */
char *construct_query(const char *name, int *length)
{
  struct datagram_header *header;
  
  char *datagram;
  char *question;

  // Allocate space for the query datagram and set some pointers into it.
  *length  = sizeof(struct datagram_header) + strlen(name) + 2 + 4;
  datagram = (char *)malloc(*length);
  if (datagram == NULL) return NULL;   // Check for out-of-memory condition.
  question = datagram + sizeof(struct datagram_header);

  // Fill in the header.
  header = (struct datagram_header *)datagram;
  // ...
  
  // Fill in the question.
  // ...

  // Fill in the query type and the query class.
  // ...
  
  return datagram;
}


//! Send a DNS query
/*!
 *  This function sends a DNS query to the specified server. It does not wait for a response
 *  from the server. No error indications are returned; it assumes that the send is successful.
 * 
 *  \param socket_handle The handle to a previously opened UDP socket.
 *  \param server_ip A pointer to a string containing the DNS server's IP address. This string
 *  should be in conventional dotted decimal notation (for example: "155.42.16.33").
 *  \param datagram A pointer to a DNS query datagram with complete headers. No check is done
 *  on the format or sanity of the datagram.
 *  \param length The number of bytes in the datagram pointed at by 'datagram.'
 */
void send_query(SOCKET socket_handle, const char *server_ip, const char *datagram, int length)
{
  struct sockaddr_in server_address;

  // ...
}


//! Wait for a DNS response.
/*!
 *  This function reads a DNS response from the given socket. If no response is received in 10
 *  seconds, this function returns a timeout indication.
 *  
 *  \param socket_handle The handle to a previously opened UDP socket.
 *  \param datagram Pointer to a variable that receives a pointer to the incoming datagram. The
 *  memory used for the datagram is dynamically allocated; the caller should free this memory
 *  when it is no longer needed (even if this function returns unsuccessfully).
 *  \param length Pointer to a variable that receives the size of the incoming datagram.
 *  
 *  \return Zero if successful, -1 if a timeout occurs, -2 if a Winsock error occurs, or -3
 *  if an out-of-memory error occurs. If an error occurs the value of '*length' is undefined.
 */
int recv_response(SOCKET socket_handle, char **datagram, int *length)
{
  struct sockaddr_in server_address;
  
  int address_length = sizeof(struct sockaddr_in);

  // What is the biggest UDP datagram that the name server might send to us? The RFC states that
  // DNS datagrams are limited to 512 bytes.
  // 
  *datagram = (char *)malloc(512);
  if (*datagram == NULL) return -3;  // Check for out-of-memory condition.

  // Receive the datagram from the server with a 10 second timeout.
  // ...
  
  return 0;
}


//! Display the results of a DNS query.
/*!
 *  This function displays the answer returned by the DNS server in the response datagram.
 *  
 *  \param datagram Pointer to the response datagram.
 *  \param length The number of bytes in the datagram pointed at by 'datagram.'
 */
void display_response(char *datagram, int length)
{
  // ...
}


// ============
// Main Program
// ============

int main(int argc, char **argv)
{
  int    return_code;      // Holds exit status returned by this program.
  char  *query;            // Points at the query datagram.
  char  *response;         // Points at the response datagram.
  int    query_length;     // The number of bytes in the query datagram.
  int    response_length;  // The number of bytes in the response datagram.
  SOCKET socket_handle;

  // Do I have the command line arguments I need?
  if (argc != 3) {
    fprintf(stderr, "*** Usage: %s server-IP name-to-query\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Try to initialize Winsock.
  if (initialize_winsock() == -1) {
    fprintf(stderr, "*** Can't find or initialize a suitable Winsock library\n");
    return EXIT_FAILURE;
  }
  
  // Create an appropriate socket.
  if ((socket_handle = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
    ErrorMessage();
    WSACleanup();
    return EXIT_FAILURE;
  }

  // Build the query datagram.
  if ((query = construct_query(argv[2], &query_length)) == NULL) {
    fprintf(stderr, "*** Out of memory while building DNS query!\n");
    WSACleanup();
    return EXIT_FAILURE;
  }

  // Send the query datagram.
  send_query(socket_handle, argv[1], query, query_length);
  free(query);

  // Get the response datagram.
  switch (recv_response(socket_handle, &response, &response_length)) {
    case -3:
      fprintf(stderr, "*** Out of memory while receiving DNS response!\n");
      return_code = EXIT_FAILURE;
      break;
      
    case -2:
      ErrorMessage();
      return_code = EXIT_FAILURE;
      break;
      
    case -1:
      fprintf(stderr, "*** Timed out waiting for DNS response!\n");
      return_code = EXIT_FAILURE;
      break;
      
    case 0:
      display_response(response, response_length);
      return_code = EXIT_SUCCESS;
      break;
      
    default:
      fprintf(stderr, "*** Unknown error occured while receiving DNS response!\n");
      return_code = EXIT_FAILURE;
      break;
  }
  free(response);

  closesocket(socket_handle);
  WSACleanup();
  return return_code;
}

