#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char *s);
void reverse(char *s);
void itoa(int n, char *s, int radix);

#endif