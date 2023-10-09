#
# Makefile for the file_services RPC server.
#

all:	client server

client: client.o file_services_clnt.o file_services_xdr.o
	gcc -o client client.o file_services_clnt.o file_services_xdr.o

client.o:	client.c file_services.h
	gcc -c -Wall client.c

server:	server.o file_services_svc.o file_services_xdr.o
	gcc -o server server.o file_services_svc.o file_services_xdr.o

server.o:	server.c file_services.h
	gcc -c -Wall server.c

file_services_clnt.o:	file_services_clnt.c file_services.h
	gcc -c file_services_clnt.c

file_services_svc.o:	file_services_svc.c file_services.h
	gcc -c file_services_svc.c

file_services_xdr.o:	file_services_xdr.c file_services.h
	gcc -c file_services_xdr.c

file_services.h:	file_services.x
	rpcgen file_services.x

file_services_clnt.c:	file_services.x
	rpcgen file_services.x

file_services_svc.c:	file_services.x
	rpcgen file_services.x

file_services_xdr.c:	file_services.x
	rpcgen file_services.x

clean:
	rm -f file_services.h \
	      file_services_clnt.c file_services_svc.c file_services_xdr.c \
	      *.o client server
