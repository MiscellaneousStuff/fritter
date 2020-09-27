#include <stdint.h>
#include <string.h>

#include <fritter/kernel.h>

#include "multiboot.h"
#include "sys/vesa.h"
#include "font.h"

uint64_t framebuffer_addr;
uint32_t framebuffer_pitch;
uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint8_t framebuffer_bpp;

uint64_t addr;
uint32_t pitch;
uint32_t width;
uint32_t height;
uint8_t bpp;
uint8_t bytes_per_pixel;

void init_vesa(multiboot_t *mb_info) {
  // Set framebuffer settings
  framebuffer_addr = mb_info->framebuffer_addr;
  framebuffer_pitch = mb_info->framebuffer_pitch;
  framebuffer_width = mb_info->framebuffer_width;
  framebuffer_height = mb_info->framebuffer_height;
  framebuffer_bpp = mb_info->framebuffer_bpp;

  // Set quick variables
  addr = framebuffer_addr;
  pitch = framebuffer_pitch;
  width = framebuffer_width;
  height = framebuffer_height;
  bpp = framebuffer_bpp;
  bytes_per_pixel = bpp / 8;
}

void draw_string(uint32_t x, uint32_t y, const char *s, uint32_t color) {
  for (int i=0; *s != '\0'; s++, i++)
    draw_char(x + i*8, y, *s, color);
}

void draw_char(uint32_t x, uint32_t y, unsigned char c, uint32_t color) {
  uint8_t data;
  for (size_t row=0; row<8; row++) {
    data = (uint8_t) *(font + c*8 + row);
    for (size_t cell=0; cell<8; cell++) {
      if (data & (0x80 >> cell)) {
        putpixel(x + cell, y + row, color);
      }
    }
  }

  /*
  for (size_t i=0; i<8; i++)
    printf("%c 0b%08b\n", c, (uint8_t) *(font + c*8 + i));
  fillrect(x, y, 8, 8, color);
  */
}

void putpixel(uint32_t x, uint32_t y, uint32_t color) {
  unsigned int where = x * bytes_per_pixel + y * (width * bytes_per_pixel);
  unsigned char *screen = (unsigned char *) framebuffer_addr;
  screen[where] = color & 255;
  screen[where + 1] = (color >> 8) & 255;
  screen[where + 2] = (color >> 16) & 255;
};

void horline(uint32_t x, uint32_t y, uint32_t width, uint32_t color) {
  fillrect(x, y, width, 1, color);
}

void verline(uint32_t x, uint32_t y, uint32_t height, uint32_t color) {
  for (size_t i=0; i<height; i++) {
    putpixel(x, y+i, color);
  }
}

void fillscr(uint32_t color) {
  fillrect(0, 0, width, height, color);
}

void fillrect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
  unsigned char *where;
  uint32_t i, j;
  for (i=0; i < height; i++) {
    where = (unsigned char *) framebuffer_addr + ((y+i)*pitch+x*bytes_per_pixel);
    for (j=0; j < width; j++) {
      where[j * bytes_per_pixel] = color & 255;
      where[j * bytes_per_pixel + 1] = (color >> 8) & 255;
      where[j * bytes_per_pixel + 2] = (color >> 16) & 255; 
    }
    where += (width * bytes_per_pixel);
  }
}

void hollowrect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
  // Left then right
  verline(x, y, height, color);
  verline(x+width-1, y, height, color);

  // Top then bottom
  horline(x, y, width, color);
  horline(x, y+height-1, width, color);
}