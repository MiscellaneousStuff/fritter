#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <fritter/kernel.h>
#include <sys/mouse.h>
#include <asm/io.h>
#include <sys/idt.h>

#include "gui.h"

// Mouse Button State
bool prev_button_state[2];
bool cur_button_state[2];

uint8_t mouse_cycle = 0;
int8_t mouse_byte[3];
int8_t mouse_x = 0;
int8_t mouse_y = 0;

void mouse_handler(registers_t *regs) {
  (void) regs;
  switch(mouse_cycle) {
    case 0:
      mouse_byte[0] = inb(0x60);
      if (MOUSE_LEFT_BUTTON(mouse_byte[0])) {
        cur_button_state[0] = true;
      } else {
        cur_button_state[0] = false;
      }
      if (MOUSE_RIGHT_BUTTON(mouse_byte[0])) {
        cur_button_state[2] = true;
      } else {
        cur_button_state[2] = false;
      }
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1] = inb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2] = inb(0x60);
      mouse_x = mouse_byte[1];
      mouse_y = mouse_byte[2];
      mouse_cycle = 0;
      break;
  }
  if (mouse_cycle == 0) {
    mouse_event_t mouse_event;

    // Determine event
    if (prev_button_state[0] == false, cur_button_state[0] == true) { // Left Button Down
      mouse_event = LEFT_DOWN;
    } else if (prev_button_state[0] == true, cur_button_state[0] == false) { // Left Button Up
      mouse_event = LEFT_UP;
    } else if (prev_button_state[2] == false, cur_button_state[2] == true) { // Right Button Down
      mouse_event = RIGHT_DOWN;
    } else if (prev_button_state[2] == true, cur_button_state[2] == false) { // Right Button Up
      mouse_event = RIGHT_UP;
    }

    // Previous state becomes current state
    memcpy(prev_button_state, cur_button_state, 3);

    // Current state becomes empty
    memset(cur_button_state, 0x00, 3);

    // Notify GUI of event (NOTE: Currently hardcoded because doing this
    //                            another way requires the OS to
    //                            support message parsing)
    gui_handle_mouse(mouse_event);
  }
}

void mouse_wait(uint8_t type) {
  uint32_t time_out = 100000;
  if (type == 0) {
    while (time_out--) {
      if ((inb(0x64) & 1) == 1) {
        return;
      }
    }
    return;
  } else {
    while (time_out--) {
      if ((inb(0x64) & 2) == 0) {
        return;
      }
    }
    return;
  }
}

void mouse_write(uint8_t data) {
  // Wait to send command
  mouse_wait(1);

  // Tell mouse we're sending command
  outb(0x64, 0xD4);

  // Wait for final part ?!?!
  mouse_wait(1);

  // Now write data
  outb(0x60, data);
}

uint8_t mouse_read() {
  mouse_wait(0);
  return inb(0x60);
}

void init_mouse() {
  uint8_t status;

  // Enable auxillary mouse device
  mouse_wait(1);
  outb(0x64, 0xA8);

  // Enable interrupts
  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  status = (inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, status);

  // Use default mouse settings
  mouse_write(0xF6);
  mouse_read(); // Ack

  // Enable mouse
  mouse_write(0xF4);
  mouse_read();

  // Setup mouse handler
  register_interrupt_handler(IRQ12, mouse_handler);
}