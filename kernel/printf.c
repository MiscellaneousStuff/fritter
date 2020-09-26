#include <string.h>

#include <stdarg.h>
#include <fritter/kernel.h>

void printf(const char *s, ...) {
  char c;
  char buf[100];
  va_list arg;
  va_start(arg, s);
  
  while ((c = (char)*s) != '\0') {
    // No strings to format
    if (c != '%') {
      putc(c);
      s++;
      continue;
    }
    s++;

    // 0 padding before formatted string
    int padding = 0;
    if (*s == '0') {
      s++;
      padding = *s - '0';
      s++;
    }
    c = *s;

    // String format type
    switch (c) {
      case 's':
        printf(va_arg(arg, char *));
        break;
      case 'd':
        itoa(va_arg(arg, int), buf, 10);
        int paddingTotal = padding - strlen(buf);
        for (int i=0; i<paddingTotal; i++)
          putc('0');
        printf(buf);
        break;
      case 'x':
        itoa(va_arg(arg, int), buf, 16);
        printf(buf);
        break;
      case 'c':
        putc(va_arg(arg, int));
        break;
    }
    s++;
  }

  va_end(arg);
}