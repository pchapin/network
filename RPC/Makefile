#
# Makefile for the extended_ints RPC server.
#

all:	client server

client: client.o extended_ints_clnt.o extended_ints_xdr.o
	g++ -o client client.o extended_ints_clnt.o extended_ints_xdr.o
	
client.o:	client.cpp extended_ints.h
	g++ -c -Wall client.cpp

server:	server.o extended_ints_svc.o extended_ints_xdr.o VeryLong.o
	g++ -o server server.o extended_ints_svc.o extended_ints_xdr.o VeryLong.o
	
server.o:	server.cpp VeryLong.hpp extended_ints.h
	g++ -c -Wall server.cpp
	
VeryLong.o:	VeryLong.cpp
	g++ -c -Wall VeryLong.cpp
	
extended_ints_clnt.o:	extended_ints_clnt.c extended_ints.h
	gcc -c extended_ints_clnt.c
	
extended_ints_svc.o:	extended_ints_svc.c extended_ints.h
	gcc -c extended_ints_svc.c
	
extended_ints_xdr.o:	extended_ints_xdr.c extended_ints.h
	gcc -c extended_ints_xdr.c

extended_ints.h:	extended_ints.x
	rpcgen extended_ints.x
	
extended_ints_clnt.c:	extended_ints.x
	rpcgen extended_ints.x
	
extended_ints_svc.c:	extended_ints.x
	rpcgen extended_ints.x
	
extended_ints_xdr.c:	extended_ints.x
	rpcgen extended_ints.x

clean:
	rm -f extended_ints.h \
	      extended_ints_clnt.c extended_ints_svc.c extended_ints_xdr.c \
	      *.o client server


