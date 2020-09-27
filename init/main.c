#include <stddef.h>
#include <string.h>

#include <fritter/kernel.h>

#include "multiboot.h"
#include "vesa.h"
#include "gui.h"

#include "gdt.h"
#include "idt.h"

#include "keyboard.h"
#include "mouse.h"

#include "timer.h"
#include "rtc.h"

// 219 = block

void gets(char *s) {
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
 
int kmain(multiboot_t *mb_info) {
  // Init Global/Interrupt Descriptor Tables
  init_gdt();
  init_idt();

  // Init date/time
  read_rtc();

  // Init screen
  init_tty();

  // Welcome message
  printf("Welcome to fritter!\n");

  // Enable Interrupts
  asm volatile("sti");

  // 60 FPS
  init_timer(60);

  // Init Keyboard and Mouse Drivers
  init_keyboard_driver();
  init_mouse();

  // Init Graphics
  init_vesa(mb_info);
  init_gui();

  // Print char from user
  char cmd[100];

  // CMD loop
  for (;;) {
    // Prompt
    printf("> ");

    // Get Command
    gets(cmd);
    printf("\n");

    // Determine command
    if (strncmp(cmd, "help", strlen(cmd)) == 0) {
      printf("clear\t Clears the screen\n");
      printf("help\t Prints this help information");
    } else if (strncmp(cmd, "clear", strlen(cmd)) == 0) {
      clrscr();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
      printf(cmd+5);
    } else if (strncmp(cmd, "time", 4) == 0) {
      printf("%04d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    } else if (strncmp(cmd, "mouse", 5) == 0) {
      printf("mouse co-ords := X: %d Y: %d", mouse_x, mouse_y);
    } else if (strncmp(cmd, "vbe", 3) == 0) {
      printf("Bootloader:\t\t%s\n", mb_info->boot_loader_name);
      printf("VBE Mode:\t\t%d\n", mb_info->vbe_mode);
      printf("MEM Upper\t\t0x%x\n", mb_info->mem_upper);
      printf("MEM Lower\t\t0x%x\n", mb_info->mem_lower);
      printf("Boot Device\t\t%d\n", mb_info->boot_device);
      printf("Module Count\t\t%d\n", mb_info->mods_count);
      printf("CMD Line\t\t0x%x", mb_info->cmdline);
    } else {
      printf("%s is an unknown command. Try again.", cmd);
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