#include <fritter/wm.h>

#include "gui.h"
#include "fritter/wm.h"

void init_wm() {
  wm_window_t window = {
    10, 300, 100, 100, "Title"
  };
  render_window(&window);
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