#include <string.h>

#include <stdarg.h>
#include <fritter/kernel.h>

void printf(const char *s, ...) {
  char c;
  char buf[100];
  va_list arg;
  va_start(arg, s);
  
  while ((c = (char *)*s) != '\0') {
    if (c != '%') {
      putc(c);
      s++;
      continue;
    }
    s++;
    c = *s;
    switch (c) {
      case 's':
        printf(va_arg(arg, char *));
        break;
      case 'd':
        itoa(va_arg(arg, int), buf, 10);
        printf(buf);
        break;
      case 'x':
        itoa(va_arg(arg, int), buf, 16);
        printf(buf);
        break;
    }
    s++;
  }

  va_end(arg);
}