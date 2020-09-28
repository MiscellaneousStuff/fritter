#include <stdbool.h>
#include <string.h>
#include <sys/vesa.h>
#include <fritter/wm.h>

#include "sys/mouse.h"
#include "gui.h"

void render_window(wm_window_t *window);
void render_screen();
void render_taskbar();

void init_wm() {
  // Init Window Manager settings
  window_count = 0;
  cur_id = 0;

  // Clear windows buffer
  memset(&windows, 0, sizeof(windows));

  // Add first window
  wm_window_t windowA = { 0, 10, 300, 100, 100, "Window A" };
  add_window(&windowA);
  wm_window_t windowB = { 0, 10, 450, 100, 100, "Window B" };
  add_window(&windowB);

  // Render screen
  render_screen();
}

void add_window(wm_window_t *window) {
  window->id = cur_id++;
  windows[window_count++] = window;
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
    window->title
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
  wm_window_t *window;
  for (size_t i=0; i<MAX_WINDOW_COUNT; i++) {
    window = windows[i];
    if (window) {
      bool x_overlap = overlaps(cursor_x, window->x, window->x + window->width);
      bool y_overlap = overlaps(cursor_y, window->y, window->y + window->height);
      bool overlap = x_overlap && y_overlap;
      if (overlap && mouse_event == LEFT_CLICK) {
        switch (mouse_event) {
          case LEFT_CLICK:
            draw_alert(window->title, "Left Click");
            break;
          case RIGHT_CLICK:
            draw_alert(window->title, "Right Click");
            break;
          case LEFT_DRAG:
            // Move window here
            break;
        }
      }
    }
  }
}