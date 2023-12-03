#include <stdio.h>

int main() {
  int x = getchar();
  int y = 5 / x;
  int a[1];
  a[y] = 0; // potential out-of-bounds access when x >= 5 or x <= 0
  return 0;
}
