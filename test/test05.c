#include <stdlib.h>

int f() {
  int* arr = malloc(10 * sizeof(int));
  arr[10] = 0; // out-of-bounds
  return 0;
}
