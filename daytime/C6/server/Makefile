CC := gcc
CFLAGS := -std=c17 -g -Og \
          -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Wformat=2 -Wconversion -Wvla
# We intend to use POSIX APIs. Set the POSIX standard desired.
CFLAGS += -D_POSIX_C_SOURCE=200809L

dtserver6:	dtserver6.o
	$(CC) -o dtserver6 dtserver6.o

dtserver6.o:	dtserver6.c
	$(CC) -c $(CFLAGS) dtserver6.c

.PHONY: clean
clean:
	rm -f *.o dtserver6
