#ifndef __VESA_H__
#define __VESA_H__

#include "multiboot.h"

uint64_t framebuffer_addr;
uint32_t framebuffer_pitch;
uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint8_t framebuffer_bpp;

void init_vesa(multiboot_t* mb_info);

void putpixel(uint32_t x, uint32_t y, uint32_t color);
void horline(uint32_t x, uint32_t y, uint32_t width, uint32_t color);
void verline(uint32_t x, uint32_t y, uint32_t height, uint32_t color);
void fillscr(uint32_t color);
void fillrect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void draw_char(uint32_t x, uint32_t y, unsigned char c, uint32_t color);
void draw_string(uint32_t x, uint32_t y, const char *s, uint32_t color);
void hollowrect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void copyrect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, unsigned char *buf);
void pasterect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, unsigned char *buf);

#endif