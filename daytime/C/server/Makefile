CC := gcc
CFLAGS := -std=c17 -g -Og \
          -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Wformat=2 -Wconversion -Wvla
# We intend to use POSIX APIs. Set the POSIX standard desired.
CFLAGS += -D_POSIX_C_SOURCE=200809L

dtserver:	dtserver.o
	$(CC) -o dtserver dtserver.o

dtserver.o:	dtserver.c
	$(CC) -c $(CFLAGS) dtserver.c

.PHONY: clean
clean:
	rm -f *.o dtserver
