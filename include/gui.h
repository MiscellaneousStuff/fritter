#ifndef __GUI_H__
#define __GUI_H__

#define COLOR_WHITE     0xFFFFFF
#define COLOR_BLACK     0x000000
#define COLOR_BLUE      0x010080
#define COLOR_RED       0xFF0000
#define COLOR_CYAN      0x008080

#define BG_COLOR        0xC0C0C0
#define LIGHT_INSERT    0xDFDFDF
#define DARK_INSERT     0x808080

#define ALERT_WIDTH    300
#define ALERT_HEIGHT   120

#define WINDOW_TITLE_HEIGHT 20

#define BUTTON_HEIGHT   25
#define BUTTON_WIDTH    75

#define TASKBAR_PADDING 2
#define TASKBAR_HEIGHT  (BUTTON_HEIGHT + (TASKBAR_PADDING * 2) + 2)

#define CURSOR_SENSITIVITY      0.25
#define CURSOR_WIDTH            12
#define CURSOR_HEIGHT           21
#define CURSOR_BYTES_PER_PIXEL  4 // NOTE: This is needed until malloc() is implemented

void init_gui();
void draw_terminal_char(char c, unsigned int x, unsigned int y);
void clear_terminal();
void draw_taskbar();
void draw_button(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *text);
void draw_inverted_button(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *text);
void draw_alert(const char *title, const char *msg);
void draw_window(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *title);
void draw_label(uint32_t x, uint32_t y, const char *text, uint32_t color);
void draw_terminal();
void gui_handle_mouse();

#endif