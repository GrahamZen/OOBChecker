#include <stdio.h>

void f() {
  int arr[] = {1, 2, 3};
  int brr[2];
  return (void)brr[arr[2]]; // out-of-bounds access
}
