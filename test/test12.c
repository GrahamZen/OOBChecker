struct node {
    int val;
    struct node* next;
};

#define alloc(out, p)\
do {\
    out = (struct node*) p;\
    out->val = 0;\
} while (0)

int main() {
    int buf[15];
    struct node* n[10];
    for (int i = 0; i < 10; i++) {
        alloc(n[i], &buf[i * sizeof(struct node)]); // out-of-bounds
    }
}
