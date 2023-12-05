#include <stdio.h>

void f() {
  int arr[] = {2, 1, 0};
  int brr[] = {7, 2, 3};
  int crr[7];
  int* p = crr + arr[2];
  int** q = &p + crr[brr[arr[2]]]; // out-of-bounds
  *q[2] = 6; // out-of-bounds
}
