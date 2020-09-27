#include <stddef.h>
#include <string.h>

#include <fritter/kernel.h>

#include "multiboot.h"
#include "sys/vesa.h"
#include "gui.h"

#include "sys/gdt.h"
#include "sys/idt.h"

#include "sys/timer.h"
 
#include "sys/keyboard.h"
#include "sys/mouse.h"

#include "cli.h"

int kmain(multiboot_t *mb_info) {
  // Init Global/Interrupt Descriptor Tables
  init_gdt();
  init_idt();

  // Init date/time
  read_rtc();

  // Init screen
  init_tty();

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

  // Init CLI
  init_cli();

  // Wait for timing events
  for (;;) {
    asm volatile ("hlt");
  }

  return 0xDEADBEEF;
}