AS = nasm
ASFLAGS = -felf32
CC = i686-elf-gcc
CFLAGS = -O2 -Wall -Wextra --freestanding -I../include -c
LD = i686-elf-ld
LDFLAGS = -r -T ../misc/linker.ld

OBJS = 	asm.o cli.o font.o gdt.o gui.o idt.o keyboard.o mouse.o printf.o rtc.o \
				serial.o timer.o tty.o vesa.o wm.o

all: kernel.o

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	
kernel.o: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.o

clean:
	rm -rf *.o