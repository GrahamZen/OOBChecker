#include <stdio.h>

void f() {
  int a[2];
  int* b = a + 3;
  int** c = &b;
  int*** d = &c;
  ***d = 0; // out-of-bounds access
}
