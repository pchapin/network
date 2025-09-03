CC := gcc
CFLAGS := -std=c17 -g -Og \
          -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wformat=2 -Wconversion -Wvla
# We intend to use POSIX APIs. Set the POSIX standard desired.
CFLAGS += -D_POSIX_C_SOURCE=200809L

echoserver:	echoserver.o
	$(CC) -o echoserver echoserver.o

echoserver.o:	echoserver.c
	$(CC) -c $(CFLAGS) echoserver.c

.PHONY: clean
clean:
	rm -f *.o echoserver

