#ifndef __WM_H__
#define __WM_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_COMPONENT_COUNT 1000
#define MAX_WINDOW_COUNT 64
#define MAX_WINDOW_TITLE 100
#define MAX_LABEL_TEXT 10
#define MAX_TEXTBOX_BUFFER 128

#define CLOSE_BUTTON_SIZE 16

#define BACKGROUND_COLOR COLOR_CYAN

typedef enum { BUTTON, LABEL } component_type_t;

typedef struct {
  uint32_t window_id;
  uint32_t id;
  uint32_t x;
  uint32_t y;
  component_type_t type;
  uint32_t width;
  uint32_t height;
  void *component_data;
  void *click_handler;
} wm_component_t;

typedef struct {
  char label[MAX_LABEL_TEXT];
  bool pressed;
} wm_button_t;

typedef struct {
  char label[MAX_LABEL_TEXT];
} wm_label_t;

typedef struct {
  uint32_t id;
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
  char title[MAX_WINDOW_TITLE];
  bool focused;
  uint8_t zdepth;
} wm_window_t;

wm_window_t *windows[MAX_WINDOW_COUNT];
wm_component_t *components[MAX_COMPONENT_COUNT];

size_t cur_window_id;
size_t window_count;
size_t cur_component_id;
size_t component_count;

void init_wm();
void add_window(wm_window_t *window);
void render_windows();
void wm_handle_mouse();

#endif