#include <stdbool.h>
#include <string.h>
#include <sys/vesa.h>
#include <fritter/wm.h>
#include <sys/vesa.h>

#include "sys/mouse.h"
#include "gui.h"

void render_window(wm_window_t *window);
void render_screen();
void render_taskbar();
void test_window();

void init_wm() {
  // Init Window Manager settings
  window_count = 0;
  cur_id = 0;

  // Clear windows buffer
  memset(&windows, 0, sizeof(windows));

  // Alert Window
  wm_window_t alert_window = { 0, 10, 400, 200, 100, "Title", false, 0 };
  wm_button_t button_data = { "Label" };
  wm_component_t button = { 0, 0, BUTTON, BUTTON_WIDTH, BUTTON_HEIGHT, &button_data };
  add_window(&alert_window);
  // add_component();

  // Render screen
  render_screen();
}

void add_component() {

}

void add_window(wm_window_t *window) {
  window->id = cur_id++;
  windows[window_count++] = window;
}

void delete_window(uint32_t id) {
  wm_window_t *window;
  for (size_t i=0; i<MAX_WINDOW_COUNT; i++) {
    window = windows[i];
    if (window) {
      if (window->id == id) {
        windows[i] = 0;
      }
    }
  }
}

void render_screen() {
  // Background
  fillscr(BACKGROUND_COLOR);

  // Taskbar
  render_taskbar();

  // Terminal top-most
  draw_terminal();

  // Windows
  render_windows();
}

void render_taskbar() {
  // Draw taskbar base
  draw_taskbar();

  // Draw button for each open window
  uint32_t start_button_offset = 2 + BUTTON_WIDTH + 4;
  size_t window_count = 0;
  wm_window_t *window;
  for (size_t i=0; i<MAX_WINDOW_COUNT; i++) {
    window = windows[i];
    if (window) {
      if (window->focused) {
        draw_pressed_button(
          start_button_offset + 2 + ((BUTTON_WIDTH*2 + 4) * window_count++),
          framebuffer_height - (BUTTON_HEIGHT + TASKBAR_PADDING),
          BUTTON_WIDTH * 2,
          BUTTON_HEIGHT,
          window->title
        );
      } else {
        draw_button(
          start_button_offset + 2 + ((BUTTON_WIDTH*2 + 4) * window_count++),
          framebuffer_height - (BUTTON_HEIGHT + TASKBAR_PADDING),
          BUTTON_WIDTH * 2,
          BUTTON_HEIGHT,
          window->title
        );
      }
    }
  }
}

void render_windows() {
  wm_window_t *window;
  for (size_t i=0; i<MAX_WINDOW_COUNT; i++) {
    window = windows[i];
    if (window)
      render_window(window);
  }
}

void render_window(wm_window_t *window) {
  draw_window(
    window->x,
    window->y,
    window->width,
    window->height,
    window->title,
    window->focused
  );
}

inline bool overlaps(uint32_t source, uint32_t target_start, uint32_t target_end) {
  if (source >= target_start && source <= target_end) {
    return true;
  } else {
    return false;
  }
}

void wm_handle_mouse(mouse_event_t mouse_event) {
  bool focus_change = false;
  wm_window_t *window;
  for (size_t i=0; i<MAX_WINDOW_COUNT; i++) {
    window = windows[i];
    if (window) {
      bool x_overlap = overlaps(cursor_x, window->x, window->x + window->width);
      bool y_overlap = overlaps(cursor_y, window->y, window->y + window->height);
      bool overlap = x_overlap && y_overlap;
      if (overlap && window->id == 0) {
        switch (mouse_event) {
          case LEFT_CLICK:
            draw_alert(window->title, "Left Click");
            window->focused = true;
            focus_change = true;
            render_windows();
            break;
          case RIGHT_CLICK:
            draw_alert(window->title, "Right Click");
            break;
          case LEFT_DRAG:
            draw_alert(window->title, "Left Drag");
            // Move window here
            break;
        }
      } else {
        window->focused = false;
      }
    }
  }
  if (focus_change) {
    render_taskbar();
  }
}