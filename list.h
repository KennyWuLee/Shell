typedef struct node {
    int val;
    struct node * next;
} node_t;

void push(node_t** head, int val);
int pop(node_t** head);
int remove_by_value(node_t** head, int val);
void delete_list(node_t* head);
void print_list(node_t* head, char* s);
int is_empty(node_t* head);
