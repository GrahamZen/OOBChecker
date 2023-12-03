#include <stdio.h>

void f() {
  int x = getchar();
  int y = 5 / x;
  int a[1];
  a[y] = 0; // potential out-of-bounds access when x >= 5 or x <= 0
  int* b = malloc(4 * sizeof(int));
  b[4] = 0; // out-of-bounds access
  free(b);
  int c[2];
  int* d = c + 3;
  int** e = &d;
  int*** f = &e;
  ***f = 0; // out-of-bounds access
  int arr[] = {1, 2, 3};
  int brr[2];
  return brr[arr[2]]; // out-of-bounds access
}
