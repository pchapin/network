
CC := gcc
CFLAGS := -std=c17 -g -Og \
          -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Wformat=2 -Wconversion -Wvla
# We intend to use POSIX APIs. Set the POSIX standard desired.
CFLAGS += -D_POSIX_C_SOURCE=200809L

dtclient:	dtclient.o
	$(CC) -o dtclient dtclient.o

dtclient.o:	dtclient.c
	$(CC) -c $(CFLAGS) dtclient.c

.PHONY: clean
clean:
	rm -f *.o dtclient
