int main() {
  int a = 0;
  int b = 1;
  int c = a + 1;
  if(1) a = 2;
  int e[1];
  e[a] = 0; // out of bounds
  return 0;
}
