AR = ar
CC = i686-elf-gcc
CFLAGS = -O2 -Wall -Wextra --freestanding -I../include -c

OBJS = string.o

lib.a: $(OBJS)
	$(AR) rcs lib.a $(OBJS)

clean:
	rm -rf *.o *.a