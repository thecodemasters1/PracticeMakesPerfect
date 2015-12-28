#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include "dynamic_list.h"
/*creates a node and sets next and prev as null*/
Node * Node_init(size_t size, void *data) {
	Node * n = malloc(size);
	n->next = NULL;
	n->previous = NULL;

	memcpy(&n->data, data, size);
}
/*delete a node*/
void Node_remove(Node * n) {
	free(n);
}
/*set node next value*/
void Node_set_next(void * current, void *next) {
	((Node *)current)->next = next;
}
/*set node previous value */
void Node_set_prev(void * current, void *prev) {
	((Node*)current)->previous = prev;
}
/*get node content void pointer , you need to cast it into the type you saved in the node*/
void* Node_get_content(void *node) {
	if (node != NULL) {
		return ((Node*)node)->data;
	}
	return NULL;
}
/*set node content*/
void Node_set_content(void *node, void *data) {
	((Node*)node)->data = data;
}

/*List control functions*/

/*Create a new list , if data = NULL there will be no node in the list */
List * List_create(size_t data_size, void *data) {
	List * l = malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->length = 0;
	l->node_size = data_size + (sizeof(void*) * 2);
	/*you have to state the data size , otherwise i dont know how much to allocate*/
	if (data_size == NULL) {
		return NULL;
	}
	if (data != NULL) {
		Node * n = Node_init(data_size, data);
		l->first = n;
		l->last = n;
		l->length = 1;
	}
	return l;
}
/*Add a value to the end of the list*/
void List_add_to_last(List * l, void * data) {
	Node *n = Node_init(l->node_size, data);
	/*if there is a node in the end*/
	if (l->last != NULL) {
		((Node*)l->last)->next = n;
		n->previous = l->last;
	}

	l->last = n;
	if (l->first == NULL) {
		l->first = n;
	}
	++l->length;
}
/*Add a value to the start of the list*/
void List_add_to_first(List *l, void * data) {
	Node *n = Node_init(l->node_size, data);
	n->next = l->first;
	/*if we dont have anything in the list*/
	if (l->first != NULL) {
		((Node*)l->first)->previous = n;
	}
	l->first = n;
	if (l->last == NULL) {
		l->last = n;
	}
	++l->length;
}
/*Remove the selected node from the list*/
void List_remove_node(List * l, Node * n) {
	if (l->first == n)
	{
		/*will put NULL if its the last node*/
		l->first = n->next;
	}
	else if (l->last == n) {

		l->last = n->previous;
	}
	else {
		((Node*)n->previous)->next = n->next;
		((Node*)n->next)->previous = n->previous;
	}
	--l->length;
	free(n);
}
/*Destroy the whole list*/
void List_destroy(List *l) {
	int i = 0;
	Node *n = NULL;
	n = l->first;
	while (l->length>0) {
		List_remove_node(l, n);
		n = l->first;
	}
	free(l);
	l = NULL;
}
/*Get the list length*/
int List_get_length(List  *l) {
	return l->length;
}

/*Queue control functions*/


/*Create a new Queue , if the data is null no value will be placed in the Queue*/
Queue * Queue_create(size_t data_size, void *data) {
	Queue * q = malloc(sizeof(Queue));
	q->data_max_size = data_size;
	q->length = 0;
	q->actual_length = 0;
	q->start = NULL;
	q->actual_start = NULL;
	if (data_size == NULL) {
		//you have to state the data size , otherwise i dont know how much to allocate
		free(q);
		return NULL;
	}

	if (data != NULL) {
		q->start = malloc(data_size);
		q->actual_start = q->start;
		q->length = 1;
		q->actual_length = 1;
		memcpy(q->start, data, data_size);
	}
	return q;
}
/*Insert new data into the Queue (to the last spot)*/
void Queue_insert(Queue * q, void * data) {
	if (q->actual_start != NULL) {
		size_t new_size = q->data_max_size*(q->actual_length + 1);
		q->actual_start = realloc(q->actual_start, new_size);
		void * last = (char *)q->actual_start + (q->actual_length*q->data_max_size); // cast the void pointer into char (char = 1 byte)
		memcpy(last, data, q->data_max_size);
		++q->length;
		++q->actual_length;
	}
	else { //if we have no data
		q->start = malloc(q->data_max_size);
		q->actual_start = q->start;
		q->length = 1;
		q->actual_length = 1;
		memcpy(q->start, data, q->data_max_size);
	}
}
/*Returns the first node and removes it form the Queue , it does not actualy remove it from the memory*/
void * Queue_pop(Queue * q) {
	if (q->length == 0) {
		return NULL;
	}
	void * first_element = q->start;
	q->start = (char*)q->start + q->data_max_size;
	--q->length;
	return first_element;
}
/*Gets the first value in the Queue , returns a void pointer to the value*/
void * Queue_get_first(Queue * q) {
	if (q->length == 0) {
		return NULL;
	}
	return q->start;
}
/*Destroys the Queue */
void Queue_destroy(Queue *q) {
	free(q->actual_start);
	free(q);
}
/*Get the Queues length*/
int Queue_get_length(Queue *q) {
	return q->length;
}