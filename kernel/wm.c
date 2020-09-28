#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <sys/vesa.h>
#include <fritter/wm.h>
#include <fritter/kernel.h>
#include <sys/vesa.h>

#include "sys/mouse.h"
#include "gui.h"

void render_window(wm_window_t *window);
void render_screen();
void render_taskbar();
inline void test_window(uint32_t window_id, uint32_t x, uint32_t y, const char *title);
void delete_window(uint32_t id);
void add_component(wm_component_t *component);
inline void first_window(uint32_t window_id, uint32_t x, uint32_t y, const char *title);
inline void second_window(uint32_t window_id, uint32_t x, uint32_t y, const char *title);

void init_wm() {
  // Init Window Manager settings
  window_count = 0;
  cur_window_id = 0;
  cur_component_id = 0;

  // Clear windows and components buffer
  memset(&windows, 0, sizeof(windows));
  memset(&components, 0, sizeof(components));

  // Test Window
  first_window(0, 100, 300, "First");
  second_window(1, 100, 450, "Second");

  // Render screen
  render_screen();
}

void btn_ok_handler(wm_window_t *window) {
  printf("OK BTN, WINDOW: %s\n", window->title);
}

void btn_cancel_handler(wm_window_t *window) {
  printf("CANCEL BTN, WINDOW: %s\n", window->title);
}

void close_handler(wm_window_t *window) {
  delete_window(window->id);
  render_screen();
}

void window_click_handler(wm_window_t *window) {
  window->focused = true;
}

inline void first_window(uint32_t window_id, uint32_t x, uint32_t y, const char *title) {
  // Window
  static wm_window_t alert_window = { 0, 0, 0, 200, 100, "", false, 0 };
  alert_window.id = window_id;
  alert_window.x = x;
  alert_window.y = y;
  memcpy(alert_window.title, title, strlen(title)+1);

  /*
  // Close button
  draw_button(
    ,
    ,
    CLOSE_BUTTON_SIZE,
    CLOSE_BUTTON_SIZE,
    "X"
  );
  */

  // Close Button
  static wm_button_t button_close_data = { "X", false };
  int close_button_x = alert_window.width - (WINDOW_BORDER*2 + CLOSE_BUTTON_SIZE) - 4;
  int close_button_y = -((WINDOW_PADDING + WINDOW_TITLE_HEIGHT) - 2); //alert_window.y + WINDOW_BORDER + 2;
  static wm_component_t button_close = { 0, 0, 0, 0, BUTTON, CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE, &button_close_data, &close_handler };
  button_close.x = close_button_x;
  button_close.y = close_button_y;
  button_close.window_id = window_id;

  // Button OK
  static wm_button_t button_ok_data = { "OK", false };
  static wm_component_t button_ok = { 0, 0, 0, 0, BUTTON, BUTTON_WIDTH, BUTTON_HEIGHT, &button_ok_data, &btn_ok_handler };
  button_ok.window_id = window_id;

  // Button Cancel
  static wm_button_t button_cancel_data = { "Cancel", false };
  static wm_component_t button_cancel = { 0, 0, BUTTON_WIDTH+5, 0, BUTTON, BUTTON_WIDTH, BUTTON_HEIGHT, &button_cancel_data, &btn_cancel_handler };
  button_cancel.window_id = window_id;

  // Add Window and Components
  add_window(&alert_window);
  add_component(&button_close);
}

inline void second_window(uint32_t window_id, uint32_t x, uint32_t y, const char *title) {
  // Window
  static wm_window_t alert_window = { 0, 0, 0, 200, 100, "", false, 0 };
  alert_window.id = window_id;
  alert_window.x = x;
  alert_window.y = y;
  memcpy(alert_window.title, title, strlen(title)+1);

  // Button OK
  static wm_button_t button_ok_data = { "OK", false };
  static wm_component_t button_ok = { 0, 0, 0, 0, BUTTON, BUTTON_WIDTH, BUTTON_HEIGHT, &button_ok_data, &btn_ok_handler };
  button_ok.window_id = window_id;

  // Button Cancel
  static wm_button_t button_cancel_data = { "Cancel", false };
  static wm_component_t button_cancel = { 0, 0, BUTTON_WIDTH+5, 0, BUTTON, BUTTON_WIDTH, BUTTON_HEIGHT, &button_cancel_data, &btn_cancel_handler };
  button_cancel.window_id = window_id;

  // Add Window and Components
  add_window(&alert_window);
  add_component(&button_ok);
  add_component(&button_cancel);
}

void add_component(wm_component_t *component) {
  component->id = cur_component_id++;
  components[component_count++] = component;
}

void add_window(wm_window_t *window) {
  window->id = cur_window_id++;
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
  // First, draw the base window
  draw_window(
    window->x,
    window->y,
    window->width,
    window->height,
    window->title,
    window->focused
  );

  // Then find and draw it's components
  wm_component_t *component;
  for (size_t i=0; i<MAX_COMPONENT_COUNT; i++) {
    component = components[i];
    wm_button_t *button = component->component_data;
    wm_label_t *label = component->component_data;
    if (component) {
      if (component->window_id == window->id) {
        // If we find a component...
        switch (component->type) {
          case BUTTON:
            draw_button(
              window->x + WINDOW_PADDING + WINDOW_BORDER + component->x,
              window->y + WINDOW_TITLE_HEIGHT + WINDOW_PADDING + WINDOW_BORDER + component->y,
              component->width,
              component->height,
              button->label
            );
            break;
          case LABEL:
            draw_label(
              window->x + WINDOW_PADDING + WINDOW_BORDER + component->x,
              window->y + WINDOW_TITLE_HEIGHT + WINDOW_PADDING + WINDOW_BORDER + component->y,
              label->label,
              COLOR_BLACK
            );
            break;
        }
      }
    }
  }
}

void wm_handle_mouse(mouse_event_t mouse_event) {
  printf("Mouse Event: %d\n", mouse_event);

  // Used to re-render taskbar if window focus changes
  bool window_focus_change = false;
  
  // Window and component holders
  wm_window_t *window;
  wm_component_t *component;

  switch (mouse_event) {
    // Mouse down events must overlap corresponding window and/or component
    case LEFT_DOWN:
    case RIGHT_DOWN:
      for (size_t i=0; i<MAX_WINDOW_COUNT; i++) {
        window = windows[i];
        if (window) {
          // Check if cursor overlaps window when clicking down on screen
          bool x_overlap_window = overlaps(cursor_x, window->x, window->x + window->width);
          bool y_overlap_window = overlaps(cursor_y, window->y, window->y + window->height);
          bool overlap_window = x_overlap_window && y_overlap_window;
          
          // Window does overlap...
          if (overlap_window) {

            // Switch focus to window
            window->focused = true;
            window_focus_change = true;

            // Dispatch events to window
            // TODO: Event dispatching for a window

            // Dispatch events to relevant components
            for (size_t j=0; j<MAX_COMPONENT_COUNT; j++) {
              component = components[j];
              if (component) {
                // Make sure component belongs to this window
                if (component->window_id == window->id) {
                  // Check if cursor overlaps component when clicking down on window
                  int component_relative_x = cursor_x-window->x-WINDOW_BORDER-WINDOW_PADDING;
                  int component_relative_y = cursor_y-window->y-WINDOW_TITLE_HEIGHT-WINDOW_BORDER-WINDOW_PADDING;
                  bool x_overlap_component = overlaps(component_relative_x, component->x, component->x + component->width);
                  bool y_overlap_component = overlaps(component_relative_y, component->y, component->y + component->height);
                  bool overlap_component = x_overlap_component && y_overlap_component;

                  // Component does overlap...
                  if (overlap_component) {

                    // Dispatch left down
                    if (mouse_event == LEFT_DOWN && component->left_down_handler != NULL) {
                      void (*left_down_handler)(wm_window_t *) = (component->left_down_handler);
                      left_down_handler(window);
                    }

                    // Dispatch mouse right down
                    // TODO: Dispatch mouse right down
                  }
                }
              }
            }
          } else {
            window->focused = false;
          }
        }
      }
      break;
  }
  if (window_focus_change) {
    render_taskbar();
    render_windows();
  }
}

/*
SWITCH (MOUSE_EVENT)
  CASE LEFT_DOWN
  CASE RIGHT_DOWN
    FOR EACH WINDOW AS W WHICH OVERLAPS MOUSE (MOUSE DOWN)
      FOR EACH WINDOW EVENT AS WE
        TRIGGER WE
      FOR EACH COMPONENT AS C WHICH OVERLAPS MOUSE (MOUSE DOWN)
        FOR EACH COMPONENT EVENT AS CE
          TRIGGER CE
  CASE LEFT_DRAG
  CASE RIGHT_DRAG
  CASE LEFT_UP
  CASE RIGHT_UP
    FOR WINDOW IN FOCUS
      FOR EACH WINDOW EVENT AS WE
        TRIGGER WE
      FOR EACH COMPONENT EVENT AS CE
        TRIGGER CE
*/