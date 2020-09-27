#ifndef __MOUSE_H__
#define __MOUSE_H__

uint8_t mouse_cycle;
int8_t mouse_byte[3];
int8_t mouse_x;
int8_t mouse_y;

void init_mouse();

#endif