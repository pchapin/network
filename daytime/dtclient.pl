#!/usr/bin/perl -w
###########################################################################
# FILE          : dtclient.pl
# LAST REVISION : January 6, 2002
# SUBJECT       : Simple TCP client program in Perl
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

use strict;
use Socket;

my ($remote, $port, $iaddr, $paddr, $proto, $line);

$remote = shift || "localhost";
$port   = shift || 13;

$iaddr  = inet_aton($remote);
$paddr  = sockaddr_in($port, $iaddr);
$proto  = getprotobyname("tcp");

socket(SOCK, PF_INET, SOCK_STREAM, $proto) or die "socket: $!";
connect(SOCK, $paddr) or die "connect: $!";
while ($line = <SOCK>) {
    print $line;
}

close(SOCK);
