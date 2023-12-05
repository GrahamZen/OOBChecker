#include <stdio.h>

void f() {
    int** arr = malloc(10 * sizeof(int*));
    for (int i = 0; i < 10; i++) {
        arr[i] = malloc(10 * sizeof(int));
    }
    int a[] = {2, 1, 0, 0, 0, 0, 0, 0, 2, 0};
    int b[] = {7, 2, 3};
    struct holder {
        int* p;
        int** q;
    };
    arr[a[b[a[2]]]][b[a[2]]] = 6; // ok
}
