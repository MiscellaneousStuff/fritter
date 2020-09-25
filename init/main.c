#include <stdint.h>
#include <stddef.h>

#include <asm/io.h>

#include <fritter/kernel.h>

void kmain() {
  init_tty();
  printf("Hello, World!");
}