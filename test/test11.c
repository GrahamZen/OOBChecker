int main() {
  int buf[3 * sizeof(int)];
  struct node {
    int id;
    int next;
  };
  struct node* n = (struct node*) &buf;
  n->id = 0;
  n->next = 1; // ok
}
