int main() {
  int a[20];
  int* b = a + 10;
  int c = 2;
  int d = 3; 
  b[c < d] = 0; // out-of-bounds access
  return 0;
}
