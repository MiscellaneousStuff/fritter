#include <fritter/kernel.h>

#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "timer.h"

int kmain() {
  // Init Global/Interrupt Descriptor Tables
  init_gdt();
  init_idt();

  // Init screen
  init_tty();

  printf("Welcome to fritter!\n> ");
  // printf("> \n");

  // Enable Interrupts
  asm volatile("sti");

  // Init timer at 100Hz
  init_timer(100);

  // Init Keyboard Driver
  init_keyboard_driver();

  // Print char from user
  for (;;) {
    char c = keyboard_getchar();
    if (c)
      printf("%c", c);
  }

  // Wait for timing events
  for (;;) {
    asm volatile ("hlt");
  }

  return 0xDEADBEEF;
}