#include <stdio.h>
#include <stdlib.h>

void f() {
  int n;
  scanf("%d", &n);
  int* arr = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    arr[i] = i; // ok
  }
  free(arr);  
}
