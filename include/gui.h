#ifndef __GUI_H__
#define __GUI_H__

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
void draw_cursor();

#endif