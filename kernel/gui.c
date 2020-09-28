#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "sys/rtc.h"
#include "gui.h"
#include "cli.h"
#include "sys/vesa.h"
#include "sys/mouse.h"

#define COLOR_WHITE     0xFFFFFF
#define COLOR_BLACK     0x000000
#define COLOR_BLUE      0x010080
#define COLOR_RED       0xFF0000
#define COLOR_CYAN      0x008080

#define BG_COLOR        0xC0C0C0
#define LIGHT_INSERT    0xDFDFDF
#define DARK_INSERT     0x808080

#define ALERT_WIDTH    300
#define ALERT_HEIGHT   120

#define WINDOW_TITLE_HEIGHT 20

#define BUTTON_HEIGHT   25
#define BUTTON_WIDTH    75

#define TASKBAR_PADDING 2
#define TASKBAR_HEIGHT  (BUTTON_HEIGHT + (TASKBAR_PADDING * 2) + 2)

#define CURSOR_SENSITIVITY      0.25
#define CURSOR_WIDTH            12
#define CURSOR_HEIGHT           21
#define CURSOR_BYTES_PER_PIXEL  4 // NOTE: This is needed until malloc() is implemented

int cursor_x = 200;
int cursor_y = 400;

unsigned char cursor_sprite_buf[CURSOR_WIDTH * CURSOR_HEIGHT * CURSOR_BYTES_PER_PIXEL]; // NOTE: 4 is bytes per pixel

// Black, White, Transparent, End of Line
// 11, 00, 01, 10

const uint8_t cursor[55] = {
  0b11100000,
  0b11111000,
  0b11001110, // 3
  
  0b11000011, 0b10000000,
  0b11000000, 0b11100000,
  0b11000000, 0b00111000, // 6 += 9
  
  0b11000000, 0b00001110,
  0b11000000, 0b00000011, 0b10000000,
  0b11000000, 0b00000000, 0b11100000, // 8 += 17

  0b11000000, 0b00000000, 0b00111000,
  0b11000000, 0b00000000, 0b00001110,
  0b11000000, 0b00000000, 0b00000011, 0b10000000, // 10 += 27

  0b11000000, 0b00000011, 0b11111111, 0b10000000,
  0b11000000, 0b11000011, 0b10000000,
  0b11000011, 0b01110000, 0b11100000, // 10 += 37

  0b11001101, 0b01110000, 0b11100000,
  0b11110101, 0b01011100, 0b00111000,
  0b01010101, 0b01011100, 0b00111000, // 9 += 46

  0b01010101, 0b01010111, 0b00001110, // 9 += 55
  0b01010101, 0b01010111, 0b00001110,
  0b01010101, 0b01010101, 0b11111000
};

void init_gui() {
  render_gui();
  copyrect(cursor_x, cursor_y, CURSOR_WIDTH, CURSOR_HEIGHT, cursor_sprite_buf);
}

void render_gui() {
  // Background
  fillscr(COLOR_CYAN);

  // Taskbar
  draw_taskbar();

  // Example Window
  draw_alert("Example Alert", "Serious stuff has happened...");

  // Terminal top-most
  draw_terminal();
}

void render_cursor() {
  // Render cursor top most
  draw_cursor(cursor_x, cursor_y);
}

void gui_handle_mouse() {
  // Clear underneath mouse first
  pasterect(cursor_x, cursor_y, CURSOR_WIDTH, CURSOR_HEIGHT, cursor_sprite_buf);

  // Calculate new mouse position
  int delta_x = mouse_x * CURSOR_SENSITIVITY;
  int delta_y = (-mouse_y) * CURSOR_SENSITIVITY; // NOTE: mouse_y is negative because mouse y delta is flipped
  int new_cursor_x = cursor_x + delta_x;
  int new_cursor_y = cursor_y + delta_y;
  if (new_cursor_x < framebuffer_width && new_cursor_x > 0) {
    cursor_x = new_cursor_x;
  }
  if (new_cursor_y < framebuffer_width && new_cursor_y > 0) {
    cursor_y = new_cursor_y;
  } 

  // Render the cursor at its new position
  render_cursor();
}

void draw_cursor(uint32_t x, uint32_t y) {
  // Save what was rendered underneath the cursor so we can replace it when we move the cursor again later
  copyrect(x, y, CURSOR_WIDTH, CURSOR_HEIGHT, cursor_sprite_buf);

  // Render the cursor
  int len = sizeof(cursor);
  int x_offset = 0;
  int y_offset = 0;
  uint8_t cursor_byte;
  for (int i=0; i<len; i++) {
    cursor_byte = (uint8_t) *(cursor + i);
    for (int j=0; j<4; j++) {
      // 0b11000000
      uint8_t current_pixel = ((cursor_byte & (0xC0 >> j*2)) >> ((3-j)*2));
      if (current_pixel == 0b11) { // Black
        putpixel(x + x_offset, y + y_offset, COLOR_BLACK);
        x_offset++;
      } else if (current_pixel == 0b00) { // White
        putpixel(x + x_offset, y + y_offset, COLOR_WHITE);
        x_offset++;
      } else if (current_pixel == 0b10) { // End of Line
        x_offset = 0;
        y_offset++;
        break;
      } else if (current_pixel == 0b01) { // Transparent
        x_offset++;
      }
    }
  }
}

void clear_terminal() {
  // Draw background
  fillrect(5+5, 5+WINDOW_TITLE_HEIGHT + 5, 80*8, 25*8, COLOR_BLACK);
}

void draw_terminal_char(char c, unsigned int x, unsigned int y) {
  int x_base = 5 + 5;
  int y_base = WINDOW_TITLE_HEIGHT + 5 + 5;
  int x_val = x_base + x*8;
  int y_val = y_base + y*8;
  if (c == '\b') {
    fillrect(x_val, y_val, 8, 8, COLOR_BLACK);
  } else {
    draw_char(x_val, y_val, c, COLOR_WHITE);
  }
}

void draw_terminal() {
  // Draw window
  draw_window(5, 5, (80*8) + 10, (WINDOW_TITLE_HEIGHT+10) + (25*8), "Terminal");

  // Draw Background
  clear_terminal();
}

void draw_alert(const char *title, const char *msg) {
  uint32_t window_x = (framebuffer_width - ALERT_WIDTH) / 2;
  uint32_t window_y = (framebuffer_height - ALERT_HEIGHT) / 2;

  uint32_t left_button_x = window_x + (ALERT_WIDTH / 2) - (3 + BUTTON_WIDTH);
  uint32_t right_button_x = window_x + (ALERT_WIDTH / 2) + 3;
  uint32_t button_y = (window_y + ALERT_HEIGHT) - BUTTON_HEIGHT - 6;

  // Draw window first
  draw_window(window_x, window_y, ALERT_WIDTH, ALERT_HEIGHT, "Hello, World!");

  // Draw message
  draw_label(window_x + 10, window_y + 10 + WINDOW_TITLE_HEIGHT, msg, COLOR_BLACK);
  
  // Then draw buttons on top
  draw_button(left_button_x, button_y, BUTTON_WIDTH, BUTTON_HEIGHT, "OK");
  draw_button(right_button_x, button_y, BUTTON_WIDTH, BUTTON_HEIGHT, "Cancel");
}

void draw_label(uint32_t x, uint32_t y, const char *text, uint32_t color) {
  draw_string(x, y, text, color);
}

void draw_window(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *title) {
  // Draw background
  fillrect(x, y, width, height, BG_COLOR);

  // Draw resizable border
  hollowrect(x+1, y+1, width-2, height-2, COLOR_WHITE); // White border
  verline(x+width-1, y, height, COLOR_BLACK); // Right-most border
  verline(x+width-2, y+1, height, DARK_INSERT); // Right-most border
  horline(x, y+height-1, width, COLOR_BLACK); // Bottom-most border
  horline(x+1, y+height-2, width-2, DARK_INSERT); // Inner bottom border

  // Draw titlebar background and text
  fillrect(x+3, y+3, width-6, WINDOW_TITLE_HEIGHT, COLOR_BLUE);
  draw_label(x+9, y+9, title, COLOR_WHITE);
}

void draw_taskbar() {
  // Draw Background
  fillrect(0, framebuffer_height-TASKBAR_HEIGHT, framebuffer_width, TASKBAR_HEIGHT, BG_COLOR);

  // Draw Resizable Line
  horline(0, framebuffer_height-TASKBAR_HEIGHT, framebuffer_width, COLOR_WHITE);

  // Draw Start Button
  draw_button(2, framebuffer_height - (BUTTON_HEIGHT + TASKBAR_PADDING), BUTTON_WIDTH, BUTTON_HEIGHT, "start");

  // Draw Time Button
  char buf[100];
  itoa(year, buf, 10);
  draw_inverted_button(framebuffer_width - BUTTON_WIDTH - 2, framebuffer_height - BUTTON_HEIGHT - 2, BUTTON_WIDTH, BUTTON_HEIGHT, buf);
}

void draw_button(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *text) {
  // Background
  fillrect(x, y, width, height, BG_COLOR);

  // Draw left and inner left
  verline(x, y, height-1, COLOR_WHITE);
  verline(x+1, y+1, height-2, LIGHT_INSERT);

  // Draw top and inner top
  horline(x, y, width-1, COLOR_WHITE);
  horline(x+1, y+1, width-3, LIGHT_INSERT);

  // Draw bottom and inner bottom
  horline(x, y+height-1, width, COLOR_BLACK);
  horline(x+1, y+height-2, width-2, DARK_INSERT);

  // Draw right and inner right
  verline(x+width-1, y, height, COLOR_BLACK);
  verline(x+width-2, y+1, height-2, DARK_INSERT);

  // If text isn't empty, render it in the center of the button
  size_t label_len = strlen(text);
  if (label_len > 0) {
    int label_x = x + (((x + width) - x) / 2) - ((label_len/2) * 8);
    int label_y = y + (((y + height) - y) / 2) - 4;
    draw_label(label_x, label_y, text, COLOR_BLACK);
  }
}

void draw_inverted_button(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *text) {
  // Draw Border
  horline(x, y, width, DARK_INSERT);
  verline(x, y, height, DARK_INSERT);
  verline(x+width-1, y, height, COLOR_WHITE);
  horline(x, y+height-1, width, COLOR_WHITE);

  // If text isn't empty, render it in the center of the button
  size_t label_len = strlen(text);
  if (label_len > 0) {
    int label_x = x + (((x + width) - x) / 2) - ((label_len/2) * 8);
    int label_y = y + (((y + height) - y) / 2) - 4;
    draw_label(label_x, label_y, text, COLOR_BLACK);
  }
}