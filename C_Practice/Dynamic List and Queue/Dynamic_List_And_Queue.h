
typedef struct {
	void * data;
	void * next;
	void * previous;
}Node;

typedef struct {
	size_t node_size;
	Node *first;
	Node *last;
	int length;
}List;

typedef struct {
	size_t data_max_size;
	void * start;
	void * actual_start;
	int length;
	int actual_length;
}Queue;
/*Node control functions*/
Node * Node_init(size_t size, void *data);

void Node_set_next(void * current, void *next);

void Node_set_prev(void * current, void *prev);

void* Node_get_content(void *node);

void Node_set_content(void *node, void *data);

/*List control functions*/
List * List_create(size_t data_size, void *data);

void List_add_to_last(List * l, void * data);

void List_add_to_first(List *l, void * data);

void List_remove_node(List * l, Node * n);

void List_destroy(List *l);

int List_get_length(List *l);

/*Queue control functions*/

Queue * Queue_create(size_t data_size, void *data);

void Queue_insert(Queue * q, void * data);

void * Queue_pop(Queue * q);

void * Queue_get_first(Queue * q);

void Queue_destroy(Queue *q);

int Queue_get_length(Queue *q);