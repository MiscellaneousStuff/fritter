CC = i686-elf-gcc
CFLAGS = -O2 -Wall -Wextra --freestanding -c
LD = i686-elf-ld
LDFLAGS = -r -T ../misc/linker.ld

OBJS = main.o

all: init.o

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	
init.o: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o init.o

clean:
	rm -rf *.o