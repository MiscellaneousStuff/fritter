#include <fritter/kernel.h>

void printf(const char *s) {
  for (int i=0; *s != '\0'; s++, i++) {
    putc(*s);
  }
}