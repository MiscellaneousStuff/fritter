AS = nasm
ASFLAGS = -felf32
LD = i686-elf-ld
LDFLAGS = -r -T ../misc/linker.ld

OBJS = head.o

all: boot.o

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

boot.o: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o boot.o

clean:
	rm -rf *.o