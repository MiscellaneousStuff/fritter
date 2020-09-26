#include <fritter/kernel.h>

#include "gdt.h"
#include "idt.h"

void kmain() {
  init_gdt();
  init_idt();

  init_tty();

  printf("Welcome to fritter!\n");
  printf("> ");

  asm volatile ("int $0x03");
}