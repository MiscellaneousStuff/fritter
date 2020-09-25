#define __LIBRARY__

#include <stdint.h>
#include <stddef.h>

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