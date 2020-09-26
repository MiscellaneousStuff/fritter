#define __LIBRARY__

#include <string.h>

#include <stdint.h>
#include <stddef.h>

int strncmp(const char *str1, const char *str2, size_t num) {
  for (size_t i=0; i<num; i++) {
    char a = str1[i];
    char b = str2[i];
    if (a != b)
      return a - b;
  }
  return 0;
}

void *memset(void *b, int c, int len) {
  //int i;
  unsigned char *p = b;
  //i = 0;
  while (len > 0) {
    *p = c;
    p++;
    len--;
  }
  return (b);
}

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s != '\0') {
    s++;
    len++;
  }
  return len;
}

void reverse(char *s) {
  int i, j;
  char c;
  for (i=0, j=strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char *s, int radix) {
  const char *digits = "0123456789ABCDEF";
  int i, sign;
  if ((sign=n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = digits[n % radix];
    // s[i++] = n % radix + '0';
  } while ((n /= radix) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}