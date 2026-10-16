/* Nicolescu David-Stefan - 313CA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Copyright: code.devmind.ro, Lab-02
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

#include "structures.h"
#include "queuesnstacks.h"

// creates an empty queue, ready for being used
my_queue_t *create_my_queue(void)
{
	my_queue_t *que = malloc(sizeof(*que));
	DIE(que == NULL, "malloc queue");

	que->front = NULL;
	que->back = NULL;
	que->size = 0;

	return que;
}

// the basic function, returns 1 if given queue is empty, 0 otherwise
int queue_is_empty(my_queue_t *que)
{
	if (!que) {
		return -1;
	}

	if (que->size == 0) {
		return 1;
	} else {
		return 0;
	}
}

// function that adds an element at the end of the queue
void enqueue(my_queue_t *que, void *data)
{
	if (queue_is_empty(que) == 1) {
		que->front = malloc(sizeof(my_queue_node_t));
		DIE(que->front == NULL, "malloc front queue");

		que->front->data = data;
		que->front->next = NULL;
		que->back = que->front;
	} else {
		my_queue_node_t *aux = malloc(sizeof(*aux));
		DIE(aux == NULL, "malloc aux queue");

		aux->data = data;
		aux->next = NULL;
		que->back->next = aux;
		que->back = aux;
	}
	que->size++;
}

// fucntion that removes and returns the first element from the queue
void *dequeue(my_queue_t *que)
{
	if (que->size == 0) {
		return NULL;
	}

	my_queue_node_t *aux = que->front;
	void *tmp = aux->data;

	que->front = que->front->next;
	if (que->front == NULL) {
		que->back = NULL;
	}

	free(aux);
	que->size--;

	return tmp;
}

// function that adds an element at the front of the queue
// similarily to a deque
void enqueue_front(my_queue_t *que, void *data)
{
	my_queue_node_t *node = malloc(sizeof(*node));
	DIE(node == NULL, "malloc queue front node");

	node->data = data;
	node->next = que->front;
	que->front = node;

	if (que->back == NULL) {
		que->back = node;
	}

	que->size++;
}

// finds the wanted priority queue
my_queue_t *get_priority_queue(system_t *sys, char *prior)
{
	if (strcmp(prior, "high") == 0)
		return sys->queue_high;
	if (strcmp(prior, "medium") == 0)
		return sys->queue_medium;
	return sys->queue_low;
}

// creates empty stack
my_stack_t *create_my_stack(void)
{
	my_stack_t *sta = malloc(sizeof(*sta));
	DIE(sta == NULL, "malloc stack");

	sta->top = NULL;
	sta->size = 0;

	return sta;
}

// the basic function, analogue queue
int stack_is_empty(my_stack_t *sta)
{
	if (sta->size == 0) {
		return 1;
	} else {
		return 0;
	}
}

// pushes an element on top of the stack
void push(my_stack_t *sta, void *data)
{
	if (stack_is_empty(sta) == 1) {
		sta->top = malloc(sizeof(my_stack_node_t));
		DIE(sta->top == NULL, "malloc");

		sta->top->data = data;
		sta->top->next = NULL;
	} else {
		my_stack_node_t *aux = malloc(sizeof(*aux));
		DIE(aux == NULL, "malloc");

		aux->data = data;
		aux->next = sta->top;
		sta->top = aux;
	}
	sta->size++;
}

// pops and returns the top element
void *pop(my_stack_t *sta)
{
	if (stack_is_empty(sta) == 1) {
		return NULL;
	}

	void *rez = sta->top->data;
	my_stack_node_t *aux = sta->top;
	sta->top = aux->next;
	sta->size--;
	free(aux);

	return rez;
}

// returns the top element without removing it
void *peek(my_stack_t *sta)
{
	if (stack_is_empty(sta) == 1) {
		return NULL;
	}

	return sta->top->data;
}

// freees a whole queue
void free_queue(my_queue_t *que)
{
	my_queue_node_t *node, *tmp;

	if (que == NULL)
		return;

	node = que->front;
	while (node != NULL) {
		tmp = node;
		node = node->next;
		free(tmp);
	}

	free(que);
}

// frees a whole stack
void free_stack(my_stack_t *sta)
{
	my_stack_node_t *node, *tmp;

	if (sta == NULL)
		return;

	node = sta->top;
	while (node != NULL) {
		tmp = node;
		node = node->next;
		free(tmp);
	}

	free(sta);
}
