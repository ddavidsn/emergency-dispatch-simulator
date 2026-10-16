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
#include "functions.h"

// function that creates the circular doubly linked list with sentinel
// for all incidents
incident_dllws_t *dllws_create_incidents(void)
{
	incident_dllws_t *inc_list = malloc(sizeof(*inc_list));
	DIE(inc_list == NULL, "malloc incidents");

	inc_list->sentinel = malloc(sizeof(*inc_list->sentinel));
	DIE(inc_list->sentinel == NULL, "malloc sentinel incidents");

	inc_list->sentinel->id = 0;
	strcpy(inc_list->sentinel->priority, "low");

	inc_list->sentinel->description = malloc(strlen("test incident") + 1);
	DIE(inc_list->sentinel->description == NULL,
		"malloc description incident");

	strcpy(inc_list->sentinel->description, "test incident");
	strcpy(inc_list->sentinel->status, "solved");

	inc_list->size = 0;

	inc_list->sentinel->prev = inc_list->sentinel;
	inc_list->sentinel->next = inc_list->sentinel;

	return inc_list;
}

// function that processes a new incident, adding it at the end of the
// incident list and in the belonging queue
void add_new_incident(system_t *sys, char *command)
{
	if (sys == NULL || sys->incidents == NULL || command == NULL) {
		return;
	}

	incident_t *new = malloc(sizeof(*new));
	DIE(new == NULL, "malloc incident node");

	char *piece;
	char *start_desc;
	char *end_desc;
	int id_nr;
	char *copy_command;

	copy_command = malloc(strlen(command) + 1); // like asked for in the pdf
	DIE(copy_command == NULL, "malloc copy command");

	strcpy(copy_command, command);

	// because we know the structure of the command, we can access the
	// necessary information easily
	piece = strtok(copy_command, " \n");
	piece = strtok(NULL, " \n");
	id_nr = atoi(piece);
	piece = strtok(NULL, " \n");

	new->id = id_nr;
	strcpy(new->status, "queued");
	strcpy(new->priority, piece);

	start_desc = strchr(command, '"'); // first "
	end_desc = strrchr(command, '"'); // last "

	if (!start_desc || !end_desc || start_desc == end_desc) {
		free(new);
		free(copy_command);
		return;
	}

	start_desc++;
	new->description = malloc(end_desc - start_desc + 1);
	DIE(new->description == NULL, "malloc description");

	strncpy(new->description, start_desc, end_desc - start_desc);
	new->description[end_desc - start_desc] = '\0';

	new->next = sys->incidents->sentinel;
	new->prev = sys->incidents->sentinel->prev;
	sys->incidents->sentinel->prev->next = new;
	sys->incidents->sentinel->prev = new;
	sys->incidents->size++;

	// finding the wanted queue
	if (strcmp(new->priority, "high") == 0) {
		enqueue(sys->queue_high, new);
	} else if (strcmp(new->priority, "medium") == 0) {
		enqueue(sys->queue_medium, new);
	} else {
		enqueue(sys->queue_low, new);
	}

	free(copy_command);
}

// given an specific id, the function finds the incident in the list
incident_t *find_incident(system_t *sys, int id)
{
	incident_t *it = sys->incidents->sentinel->next;

	while (it != sys->incidents->sentinel) {
		if (it->id == id)
			return it;
		it = it->next;
	}

	return NULL;
}

// function used for freeing a single incident node + the description
void free_incident(incident_t *node)
{
	if (node == NULL) {
		return;
	}

	free(node->description);
	free(node);
}

// function that creates the circular doubly linked list with sentinel
// for all interventions
interv_dllws_t *dllws2_create_interventions(void)
{
	interv_dllws_t *int_list = malloc(sizeof(*int_list));
	DIE(int_list == NULL, "malloc interventions");
	int_list->sentinel = malloc(sizeof(*int_list->sentinel));
	DIE(int_list->sentinel == NULL, "malloc sentinel interventions");

	int_list->sentinel->incident = NULL;
	int_list->sentinel->unit = NULL;

	int_list->size = 0;

	int_list->sentinel->prev = int_list->sentinel;
	int_list->sentinel->next = int_list->sentinel;

	return int_list;
}

// function that searches for the intervention using an id from the incident
intervention_t *find_intervention_by_incident(system_t *sys, int id)
{
	intervention_t *it = sys->interventions->sentinel->next;

	while (it != sys->interventions->sentinel) {
		if (it->incident && it->incident->id == id)
			return it;
		it = it->next;
	}

	return NULL;
}

// function used for creating the whole system
system_t *system_create(void)
{
	system_t *sys = malloc(sizeof(*sys));
	DIE(sys == NULL, "malloc system");

	sys->units = NULL;
	sys->units_nr = 0;

	sys->incidents = dllws_create_incidents();
	sys->interventions = dllws2_create_interventions();

	sys->queue_high = create_my_queue();
	sys->queue_medium = create_my_queue();
	sys->queue_low = create_my_queue();
	sys->queue_available_units = create_my_queue();

	sys->dispatches = create_my_stack();

	return sys;
}

// frees the incident list without forgetting the sentinel
void free_incident_list(incident_dllws_t *list)
{
	incident_t *node, *tmp;

	if (list == NULL)
		return;

	node = list->sentinel->next;
	while (node != list->sentinel) {
		tmp = node;
		node = node->next;
		free_incident(tmp);
	}

	free(list->sentinel->description);
	free(list->sentinel);
	free(list);
}

// frees the intervention list including sentinel
void free_intervention_list(interv_dllws_t *list)
{
	intervention_t *node, *tmp;

	if (list == NULL)
		return;

	node = list->sentinel->next;
	while (node != list->sentinel) {
		tmp = node;
		node = node->next;
		free(tmp);
	}

	free(list->sentinel);
	free(list);
}

// frees the system and all used memory
void free_system(system_t *sys)
{
	if (sys == NULL)
		return;

	free(sys->units);

	free_incident_list(sys->incidents);
	free_intervention_list(sys->interventions);

	free_queue(sys->queue_high);
	free_queue(sys->queue_medium);
	free_queue(sys->queue_low);
	free_queue(sys->queue_available_units);

	free_stack(sys->dispatches);

	free(sys);
}
