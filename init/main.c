#include <stdint.h>
#include <stddef.h>

#include <asm/io.h>

const int color = 0x07;
uint16_t *tty_buf = (uint16_t *) 0xB8000;

const int WIDTH = 80;
const int HEIGHT = 25;

// Disables the inbuilt VGA cursor
void discur() {
  outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

// Clears the screen
void clrscr() {
  for (int i=0; i<(WIDTH * HEIGHT); i++) {
    tty_buf[i] = 0;
  }
}

// Prints text to the screen
void printf(const char *s) {
  for (int i=0; *s != '\0'; s++, i++) {
    tty_buf[i] = (uint16_t) *s | (uint16_t) color << 8;
  }
}

void kmain() {
  discur();
  clrscr();
  printf("Hello, World!");
}