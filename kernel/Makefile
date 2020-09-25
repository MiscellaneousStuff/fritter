CC = i686-elf-gcc
CFLAGS = -O2 -Wall -Wextra --freestanding -I../include -c
LD = i686-elf-ld
LDFLAGS = -r -T ../misc/linker.ld

OBJS = printf.o

all: kernel.o

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	
kernel.o: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.o

clean:
	rm -rf *.o