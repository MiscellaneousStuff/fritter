#include <stddef.h>

#include "gui.h"
#include "vesa.h"

#define COLOR_WHITE     0xFFFFFF
#define COLOR_BLACK     0x000000
#define COLOR_RED       0xFF0000
#define COLOR_CYAN      0x008080

#define BG_COLOR        0xC0C0C0
#define LIGHT_INSERT    0xDFDFDF
#define DARK_INSERT     0x808080

#define BUTTON_HEIGHT   25
#define BUTTON_WIDTH    75

#define TASKBAR_PADDING 2
#define TASKBAR_HEIGHT  (BUTTON_HEIGHT + (TASKBAR_PADDING * 2) + 2)

void init_gui();
void draw_taskbar();
void draw_button();

void init_gui() {
  fillscr(COLOR_CYAN); // Red
  draw_taskbar();
}

void draw_taskbar() {
  // Draw Background
  fillrect(0, framebuffer_height-TASKBAR_HEIGHT, framebuffer_width, TASKBAR_HEIGHT, BG_COLOR);

  // Draw Resizable Line
  horline(0, framebuffer_height-TASKBAR_HEIGHT, framebuffer_width, COLOR_WHITE);

  // Draw Start Button
  draw_button(2, framebuffer_height - (BUTTON_HEIGHT + TASKBAR_PADDING), BUTTON_WIDTH, BUTTON_HEIGHT);
}

void draw_button(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
  // Background
  fillrect(x, y, width, height, BG_COLOR);

  // Draw left and inner left
  verline(x, y, height-1, COLOR_WHITE);
  verline(x+1, y+1, height-2, LIGHT_INSERT);

  // Draw top and inner top
  horline(x, y, width-1, COLOR_WHITE);
  horline(x+1, y+1, width-3, LIGHT_INSERT);

  // Draw bottom and inner bottom
  horline(x, y+height, width, COLOR_BLACK);
  horline(x+1, y+height-1, width-2, DARK_INSERT);

  // Draw right and inner right
  verline(x+width-1, y, height, COLOR_BLACK);
  verline(x+width-2, y+1, height-2, DARK_INSERT);
}