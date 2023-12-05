void f() {
  int a[20];
  int* b = a + 10;
  int c = 2;
  int d = 3; 
  if (c < d) {
    b[-1] = 0; // out-of-bounds access
  } else {
    b[1] = 0;
  }
}
