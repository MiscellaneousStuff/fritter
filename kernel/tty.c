#define TAB_SIZE 8

#include <stdint.h>
#include <asm/io.h>
#include <fritter/kernel.h>

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

// Moves the cursor to the new line (NOTE: Wraps around the screen, doesn't buffer offscreen text)
void newline() {
  x = 0;
  if (++y == HEIGHT) {
    y = 0;
  }
}

// Increments the cursor (NOTE: Wraps around the screen)
void inccur() {
  if (++x == WIDTH) {
    newline();
  }
}

// Decrements the cursor (NOTE: Wraps around the screen)
void deccur() {
  if (--x == 0) {
    x = WIDTH-1;
    if (--y == 0) {
      y = HEIGHT-1;
    }
  }
}

// Output char to a specific co-ordinate on the screen
void print_char(char c, unsigned int x, unsigned int y, unsigned char color) {
  tty_buf[y * WIDTH + x] = (uint16_t) c | (uint16_t) color << 8;
}


// Prints a character to the main screen
void putchar(char c) {
  unsigned int padding;
  switch (c) {
    case '\t':
      padding = (TAB_SIZE - (x % TAB_SIZE));
      for (unsigned int i=0; i<padding; i++)
        putchar(' ');
      break;
    case '\n':
      newline();
      break;
    case '\b':
      deccur();
      putchar(' ');
      deccur();
      break;
    default:
      print_char(c, x, y, color);
      inccur();
      break;
  }
}

// Prints a character to the screen
void putc(char c) {
  putchar(c);
  write_serial(c); // All standard output is also written to COM port 1
}