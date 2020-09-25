#include <fritter/kernel.h>

void kmain() {
  init_tty();
  printf("My name is %s and I am %d years old. 0x%x", "Joe", 20, 0x32);
}