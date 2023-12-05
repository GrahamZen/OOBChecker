#include <stdio.h>
#include <stdlib.h>
void f() {
  int n;
  scanf("%d", &n);
  int* arr = malloc(n * sizeof(int));
  arr[n+1] = 0; // out-of-bounds
}