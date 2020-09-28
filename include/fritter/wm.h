#ifndef __WM_H__
#define __WM_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_WINDOW_COUNT 64
#define MAX_WINDOW_TITLE 100
#define MAX_BUTTON_TEXT 10

#define BACKGROUND_COLOR COLOR_CYAN

enum component_type { button };

typedef struct {
  uint32_t window_id;
  uint32_t type;
  uint32_t width;
  uint32_t height;
} wm_component_t;

typedef struct {
  wm_component_t component;
  char label[MAX_BUTTON_TEXT];
} wm_button_t;

typedef struct {
  uint32_t id;
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
  char title[MAX_WINDOW_TITLE];
} wm_window_t;

wm_window_t *windows[MAX_WINDOW_COUNT];

size_t cur_id;
size_t window_count;

void init_wm();
void add_window(wm_window_t *window);
void render_windows();
void wm_handle_mouse();

#endif