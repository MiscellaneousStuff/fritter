#include <stddef.h>
#include <string.h>

#include <fritter/kernel.h>

#include "sys/rtc.h"

#include "sys/keyboard.h"
#include "sys/mouse.h"

#include "gui.h"

// ASCII 219 = block
void gets(char *s) {
  size_t n = 0;
  for (;;) {
    char c = keyboard_getchar();
    if (c) {
      if (c == '\b' && n > 0) {
        s--;
        n--;
        putc('\b');
      } else if (c == '\n') {
        *s = '\0';
        break;
      } else {
        *s++ = c;
        n++;
        putc(c);
      }
    }
  }
}

void init_cli() {
  // Welcome message
  printf("Welcome to fritter!\n");

  // Print char from user
  char cmd[1024];

  // CMD loop
  for (;;) {
    // Prompt
    printf("> ");

    // Get Command
    gets(cmd);
    printf("\n");

    // Determine command
    if (strncmp(cmd, "help", strlen(cmd)) == 0) {
      printf("clear\t Clears the screen\n");
      printf("echo\t Displays text to the screen\n");
      printf("help\t Prints this help information\n");
      printf("mouse\t Prints mouse information to the screen\n");
      printf("time\t Displays current date and time (Non-DST aware)");
    } else if (strncmp(cmd, "clear", strlen(cmd)) == 0) {
      clrscr();
      clear_terminal();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
      printf(cmd+5);
    } else if (strncmp(cmd, "time", 4) == 0) {
      printf("%04d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    } else if (strncmp(cmd, "mouse", 5) == 0) {
      printf("mouse co-ords := X: %d Y: %d", mouse_x, mouse_y);
    } else {
      printf("%s is an unknown command. Try again.", cmd);
    }

    // Newline
    printf("\n");
  }
}