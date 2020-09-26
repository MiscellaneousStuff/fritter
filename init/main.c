#include <stddef.h>
#include <string.h>

#include <fritter/kernel.h>

#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "timer.h"

// 219 = block

void *gets(char *s) {
  size_t n = 0;
  for (;;) {
    char c = keyboard_getchar();
    if (c) {
      if (c == '\b' && n > 0) {
        s--;
        n--;
        putc('\b');
      } else if (c == '\n') {
        *s = '\0';
        break;
      } else {
        *s++ = c;
        n++;
        putc(c);
      }
    }
  }
}
 
int kmain() {
  // Init Global/Interrupt Descriptor Tables
  init_gdt();
  init_idt();

  // Init screen
  init_tty();

  printf("Welcome to fritter!\n");
  // printf("> \n");

  // Enable Interrupts
  asm volatile("sti");

  // Init timer at 100Hz
  init_timer(100);

  // Init Keyboard Driver
  init_keyboard_driver();

  // Print char from user
  char cmd[100];

  // CMD loop
  for (;;) {
    // Prompt
    printf("> ");

    // Get Command
    gets(cmd);

    // Determine command
    if (strncmp(cmd, "help", strlen(cmd)) == 0) {
      printf("\n");
      printf("help - Prints this help information");
    }

    // Newline
    printf("\n");
  }

  // Wait for timing events
  for (;;) {
    asm volatile ("hlt");
  }

  return 0xDEADBEEF;
}