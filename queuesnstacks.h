/* Nicolescu David-Stefan - 313CA */

#ifndef QUEUESNSTACKS_H
#define QUEUESNSTACKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef struct system_t system_t;

// struct for a queue node
typedef struct my_queue_node_t {
	void *data; // like taught at the labs :)
	struct my_queue_node_t *next;
} my_queue_node_t;

// struct for a simple linked queue
typedef struct my_queue_t {
	my_queue_node_t *front;
	my_queue_node_t *back;
	int size;
} my_queue_t;

// all queue operations:
my_queue_t *create_my_queue(void);
int queue_is_empty(my_queue_t *que);
void enqueue(my_queue_t *que, void *data);
void *dequeue(my_queue_t *que);
void enqueue_front(my_queue_t *que, void *data);
my_queue_t *get_priority_queue(system_t *sys, char *prior);

// struct for a stack node
typedef struct my_stack_node_t {
	void *data;
	struct my_stack_node_t *next;
} my_stack_node_t;

// struct for a simple linked stack
typedef struct my_stack_t {
	my_stack_node_t *top;
	int size;
} my_stack_t;

// all stack operations:
my_stack_t *create_my_stack(void);
int stack_is_empty(my_stack_t *sta);
void push(my_stack_t *sta, void *data);
void *pop(my_stack_t *sta);
void *peek(my_stack_t *sta);

// functions for freeing the queues/stacks
void free_queue(my_queue_t *que);
void free_stack(my_stack_t *sta);

#endif
