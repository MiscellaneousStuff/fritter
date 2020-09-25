#include <fritter/kernel.h>

void kmain() {
  init_tty();
  // printf("My name is %s and I am %d years old. 0x%x\nhi", "Joe", 20, 0x32);
  for (int i=0; i<25; i++) {
    printf("%d\t%d\t%d\t\n", i, i+25, i+50);
    // printf("%d\t\n", i);
  }
}