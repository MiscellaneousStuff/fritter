#ifndef __WM_H__
#define __WM_H__

#include <stdint.h>

void init_wm();

typedef struct {
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
  char title[100];
} wm_window_t;

#endif