#include <string.h>
#include <fritter/wm.h>

#include "gui.h"

void render_window(wm_window_t *window);

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

  // Render windows
  // render_window(windows[0]);
  render_windows();
}

void add_window(wm_window_t *window) {
  window->id = cur_id++;
  windows[window_count++] = window;
}

void render_screen() {
  // Render windows first, everything else goes on top
  render_windows();
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