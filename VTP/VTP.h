
#ifndef VTP_H
#define VTP_H

#include <stdint.h>
#include <arpa/inet.h>

struct VTP_header {
  uint16_t data_length; // Is zero for control packets.
  uint16_t flags;       // 0 = data, 1 = ACK, 2 = CONN_REQ, 4 = CLOSE_REQ
};

#define VTP_DATA      0
#define VTP_ACK       1
#define VTP_CONN_REQ  2
#define VTP_CLOSE_REQ 4

#define IPPROTO_VTP   254  // Experimental protocols. See RFC-3692.

int VTP_socket( void );
  // Returns a socket handle or -1 on error.

int VTP_connect( int handle, struct sockaddr_in *remote_addr );
  // Connects given socket with specified remote address. -1 on error.

int VTP_read( int handle, void *buffer, int size );
  // Reads from given socket.

int VTP_write( int handle, void *buffer, int size );
  // Writes to given socket.

int VTP_close( int handle );
  // Closes given socket. -1 on error.

int VTP_bind( int handle, struct sockaddr_in *local_addr );
  // Associates given socket with specified local address. -1 on error.

int VTP_listen( int handle );
  // Allows given socket to establish connections. -1 on error.

int VTP_accept( int handle, struct sockaddr_in *remote_addr );
  // Waits for new connection and returns socket handle of a new socket
  // for that connection. Also provides remote address. -1 on error.

#endif
