#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>
#include <stddef.h>

void *memset(void *b, int c, int len);
size_t strlen(const char *s);
void reverse(char *s);
void itoa(int n, char *s, int radix);
int strncmp(const char *str1, const char *str2, size_t num);

#endif