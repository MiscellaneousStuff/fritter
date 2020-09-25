#include <stdint.h>
#include <stddef.h>

#include <asm/io.h>

const unsigned char color = 0x07;
uint16_t *tty_buf = (uint16_t *) 0xB8000;

const unsigned int WIDTH = 80;
const unsigned int HEIGHT = 25;

unsigned int x = 0;
unsigned int y = 0;

// Disables the inbuilt VGA cursor
void discur() {
  outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

// Clears the screen
void clrscr() {
  for (unsigned int i=0; i<(WIDTH * HEIGHT); i++) {
    tty_buf[i] = 0;
  }
}

void init_tty() {
  discur();
  clrscr();
}

// Increments the cursor
void inccur() {
  if (++x == WIDTH) {
    x = 0;
    if (++y == HEIGHT) {
      y = 0;
    }
  }
}

// Prints a character to the screen
void putc(char c) {
  tty_buf[y * WIDTH + x] = (uint16_t) c | (uint16_t) color << 8;
  inccur();
}

// Prints text to the screen
void printf(const char *s) {
  for (int i=0; *s != '\0'; s++, i++) {
    putc(*s);
  }
}

void kmain() {
  init_tty();
  printf("Hello, World!");
}