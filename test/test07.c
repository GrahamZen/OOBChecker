#include <stdio.h>
#include <stdlib.h>
void f() {
  int* x = malloc(4 * sizeof(int));
  x[4] = 0; // out-of-bounds access
  free(x);
}