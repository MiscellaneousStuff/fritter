#include <stdint.h>
#include <stddef.h>

const int color = 0x07;
uint16_t *tty_buf = (uint16_t *) 0xB8000;

const int WIDTH = 80;
const int HEIGHT = 25;

void clrscr() {
  for (int i=0; i<(WIDTH * HEIGHT); i++) {
    tty_buf[i] = 0;
  }
}

void printf(const char *s) {
  for (int i=0; *s != '\0'; s++, i++) {
    tty_buf[i] = (uint16_t) *s | (uint16_t) color << 8;
  }
}

void kmain() {
  clrscr();
  printf("Hello, World!");
}