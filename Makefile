LD = i686-elf-ld
LDFLAGS = -T misc/linker.ld

OBJS = boot/boot.o init/init.o kernel/kernel.o
LIBS = lib/lib.a

all: kernel.elf

boot/boot.o:
	(cd boot; make)

init/init.o:
	(cd init; make)

kernel/kernel.o:
	(cd kernel; make)

lib/lib.a:
	(cd lib; make)

kernel.elf: $(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o kernel.elf

run: kernel.elf
	qemu-system-i386 -kernel kernel.elf

run-iso: fritter.iso
	qemu-system-i386 -cdrom fritter.iso

fritter.iso: kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/kernel.elf
	cp misc/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o fritter.iso isodir

clean:
	(cd boot; make clean)
	(cd init; make clean)
	(cd kernel; make clean)
	(cd lib; make clean)
	rm -rf kernel.elf isodir fritter.iso