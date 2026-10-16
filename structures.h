/* Nicolescu David-Stefan - 313CA */

#ifndef STRUCTURES_H
#define STRUCTURES_H

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

#include "queuesnstacks.h"

// struct for unit, like the example in the pdf
typedef struct unit_t {
	int id;
	char type;
	int availability;
} unit_t;

// strcut for an incident node for the doubly linked list with sentinel
typedef struct incident_t {
	int id;
	char priority[7];
	char *description;
	char status[11];
	struct incident_t *prev, *next;
} incident_t;

// struct for the incident list
typedef struct incident_dllws_t {
	incident_t *sentinel;
	int size;
} incident_dllws_t;

// strcut for an intervention node for the doubly linked list with sentinel
typedef struct intervention_t {
	incident_t *incident;
	unit_t *unit;
	struct intervention_t *prev, *next;
} intervention_t;

// strcut for the intervention list
typedef struct interv_dllws_t {
	intervention_t *sentinel;
	int size;
} interv_dllws_t;

// like recommended in the pdf, I used a system structure containing
// everything for this homework, including the lists, the units, the queues
typedef struct system_t {
	unit_t *units;
	unsigned int units_nr;

	incident_dllws_t *incidents;
	interv_dllws_t *interventions;

	my_queue_t *queue_high;
	my_queue_t *queue_medium;
	my_queue_t *queue_low;
	my_queue_t *queue_available_units;

	my_stack_t *dispatches;
} system_t;

// all the functions that I'll use in the structures.c
system_t *system_create(void);
incident_dllws_t *dllws_create_incidents(void);
interv_dllws_t *dllws2_create_interventions(void);
void add_new_incident(system_t *sys, char *command);
incident_t *find_incident(system_t *sys, int id);
intervention_t *find_intervention_by_incident(system_t *sys, int id);

void free_incident_list(incident_dllws_t *list);
void free_intervention_list(interv_dllws_t *list);
void free_system(system_t *sys);

#endif
