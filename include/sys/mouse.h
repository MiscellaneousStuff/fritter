#ifndef __MOUSE_H__
#define __MOUSE_H__

#define MOUSE_LEFT_BUTTON(flag) (flag & 0x1)
#define MOUSE_RIGHT_BUTTON(flag) (flag & 0x2)
#define MOUSE_MIDDLE_BUTTON(flag) (flag & 0x4)

typedef enum { LEFT_DOWN, RIGHT_DOWN, LEFT_UP, RIGHT_UP, LEFT_DRAG, RIGHT_DRAG } mouse_event_t;

uint8_t mouse_cycle;
int8_t mouse_byte[3];
int8_t mouse_x;
int8_t mouse_y;

void init_mouse();

#endif