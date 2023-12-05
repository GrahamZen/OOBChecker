#include <stdio.h>
#include <stdlib.h>

void f() {
  int* a = malloc(10 * sizeof(int));
  int* b = a + 10;
  int c = 2;
  int d = 3;
  b[-2] = 0; // actually ok
}
