AS = nasm
ASFLAGS = -felf32
LD = i686-elf-ld
LDFLAGS = -r -T ../misc/linker.ld

all: head.o

%.o: %.asm
	$(CC) $(CFLAGS) $< -o $@

boot.o:
	$(LD) $(LDFLAGS) $(OBJS) -o init.o

clean:
	rm -rf .o