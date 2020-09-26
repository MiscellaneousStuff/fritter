#include <fritter/kernel.h>

#include "gdt.h"
#include "idt.h"
#include "timer.h"

void kmain() {
  init_gdt();
  init_idt();

  init_tty();

  printf("Welcome to fritter!\n");
  // printf("> \n");

  init_timer(100);

  // Enable interrupts and wait for them
  asm volatile("sti");
  for (;;) {}
}