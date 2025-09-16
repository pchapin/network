
CC := gcc
CFLAGS := -std=c17 -g -Og \
          -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Wformat=2 -Wconversion -Wvla
# We intend to use POSIX APIs. Set the POSIX standard desired.
CFLAGS += -D_POSIX_C_SOURCE=200809L

dtclient6:	dtclient6.o
	$(CC) -o dtclient6 dtclient6.o

dtclient6.o:	dtclient6.c
	$(CC) -c $(CFLAGS) dtclient6.c

.PHONY: clean
clean:
	rm -f *.o dtclient6
