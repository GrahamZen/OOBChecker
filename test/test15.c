#include <stdlib.h>
int main() {
  int* buf = (int*) malloc(3 * sizeof(int));
  struct node {
    int id;
    int next;
  };
  struct node* n = (struct node*) buf + 1;
  n->id = 0;
  n->next = 1; // ok
}
