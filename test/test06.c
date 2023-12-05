#include <stdlib.h>
#include <stdio.h>

void f() {
  char* str = malloc(10 * sizeof(char));
  int i = 0;
  while(getchar() != '\n' && i < 10) {
    *(str++) = 'a'; // ok
    i++;
  }
}
