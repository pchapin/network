#!/usr/bin/python
###########################################################################
# FILE          : dtclient.py
# LAST REVISION : January 6, 2002
# SUBJECT       : Simple TCP client program in Python
# PROGRAMMER    : (C) Copyright 2002 by Peter C. Chapin
#
# Please send comments or bug reports to
#
#      Peter C. Chapin
#      Computer Information Systems
#      Vermont Technical College
#      Williston, VT 05495
#      PChapin@vtc.vsc.edu
###########################################################################

from socket import *
import sys

# Get optional host and port from the command line.
port = 13
if len(sys.argv) >= 3:
    port = sys.argv[2]

host = 'localhost'
if len(sys.argv) >= 2:
    host = sys.argv[1]

# Create a socket and connect it to the server.
my_socket = socket(AF_INET, SOCK_STREAM)
my_socket.connect((host, int(port)))

# Get the server data.
print my_socket.recv(1024)

# Clean up.
my_socket.close()
