AS = nasm
ASFLAGS = -felf32
CC = i686-elf-gcc
CFLAGS = -O2 -Wall -Wextra --freestanding -I../include -c
LD = i686-elf-ld
LDFLAGS = -r -T ../misc/linker.ld

OBJS = asm.o gdt.o idt.o printf.o serial.o tty.o

all: kernel.o

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	
kernel.o: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.o

clean:
	rm -rf *.o