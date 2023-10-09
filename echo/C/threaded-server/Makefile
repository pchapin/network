
echoserver:	echoserver.o
	gcc -o echoserver echoserver.o -pthread

echoserver.o:	echoserver.c
	gcc -c -Wall echoserver.c -pthread

clean:
	rm *.o echoserver
