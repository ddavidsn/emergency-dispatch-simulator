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

#define MAX_LEN 4096

// prints the error message specified in the pdf
void print_error_message(FILE *fout)
{
	fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

void add_incident(system_t *sys, char *comanda)
{
	add_new_incident(sys, comanda);
}

// prints the current number of available units
void check_units(FILE *fout, system_t *sys)
{
	fprintf(fout, "Number of available units: %d\n",
		sys->queue_available_units->size);
}

// function that starts an intervention
// checks the highest priority, sends the first available unit
void dispatch(system_t *sys, FILE *fout)
{
	incident_t *incident = NULL;
	unit_t *unit = NULL;
	intervention_t *new_interv = NULL;
	my_queue_t *chosen_queue = NULL;

	// error case, no available units
	if (queue_is_empty(sys->queue_available_units)) {
		print_error_message(fout);
		return;
	}

	if (!queue_is_empty(sys->queue_high)) {
		chosen_queue = sys->queue_high;
	} else if (!queue_is_empty(sys->queue_medium)) {
		chosen_queue = sys->queue_medium;
	} else if (!queue_is_empty(sys->queue_low)) {
		chosen_queue = sys->queue_low;
	} else {
		print_error_message(fout); // no incidents that require attendance
		return;
	}

	incident = (incident_t *)dequeue(chosen_queue);
	unit = (unit_t *)dequeue(sys->queue_available_units);

	new_interv = malloc(sizeof(*new_interv));
	DIE(new_interv == NULL, "malloc intervention");

	new_interv->incident = incident;
	new_interv->unit = unit;

	new_interv->next = sys->interventions->sentinel;
	new_interv->prev = sys->interventions->sentinel->prev;
	sys->interventions->sentinel->prev->next = new_interv;
	sys->interventions->sentinel->prev = new_interv;
	sys->interventions->size++;

	strcpy(incident->status, "intervened");
	unit->availability = 0;

	push(sys->dispatches, new_interv);
}

// function that cancels the last unfinished dispatch
// restores the incident and the unit
void undo_lst_dispatch(system_t *sys, FILE *fout)
{
	intervention_t *interv = NULL;
	incident_t *incident = NULL;
	unit_t *unit = NULL;
	my_queue_t *queue = NULL;

	while (!stack_is_empty(sys->dispatches)) {
		interv = (intervention_t *)pop(sys->dispatches);

		if (interv->incident &&
		    strcmp(interv->incident->status, "intervened") == 0) {
			incident = interv->incident;
			unit = interv->unit;
			queue = get_priority_queue(sys, incident->priority);

			strcpy(incident->status, "queued");
			unit->availability = 1;

			enqueue_front(queue, incident);
			enqueue(sys->queue_available_units, unit);

			interv->prev->next = interv->next;
			interv->next->prev = interv->prev;
			sys->interventions->size--;

			free(interv);
			return;
		}
	}

	// error case
	print_error_message(fout);
}

// function that marks an incident as solved and makes the unit available again
void solved_incident(system_t *sys, char *command, FILE *fout)
{
	int id_to_find;

	if (sscanf(command, "%*s %d", &id_to_find) != 1) {
		print_error_message(fout);
		return;
	}

	incident_t *incident = NULL;
	intervention_t *interv = NULL;

	// error case
	if (id_to_find < 0) {
		print_error_message(fout);
		return;
	}

	incident = find_incident(sys, id_to_find);
	if (incident == NULL || strcmp(incident->status, "intervened") != 0) {
		print_error_message(fout); // error case
		return;
	}

	interv = find_intervention_by_incident(sys, id_to_find);
	if (interv == NULL) {
		print_error_message(fout); // error case
		return;
	}

	strcpy(incident->status, "solved");
	interv->unit->availability = 1;
	enqueue(sys->queue_available_units, interv->unit);
}

// function that shows details of an unit
void show_unit(system_t *sys, char *command, FILE *fout)
{
	int id_to_find;

	if (sscanf(command, "%*s %d", &id_to_find) != 1) {
		print_error_message(fout);
		return;
	}

	if (id_to_find < 0) {
		print_error_message(fout);
		return;
	}

	for (unsigned int i = 0; i < sys->units_nr; i++) {
		if (sys->units[i].id == id_to_find) {
			fprintf(fout, "Unit %d is type %c and is %s\n",
				sys->units[i].id,
				sys->units[i].type,
				sys->units[i].availability ?
				"available" : "unavailable");
			return;
		}
	}

	// error case
	print_error_message(fout);
}

// fucntion that shows details of an incident
void show_incident(system_t *sys, char *command, FILE *fout)
{
	incident_t *incident = NULL;
	int id_to_find;

	if (sscanf(command, "%*s %d", &id_to_find) != 1) {
		print_error_message(fout);
		return;
	}

	incident = find_incident(sys, id_to_find);

	if (incident == NULL) {
		print_error_message(fout); // error case
		return;
	} else {
		fprintf(fout, "Incident %d has %s priority, the following "
			"description: \"%s\" and is %s\n",
			incident->id, incident->priority,
			incident->description, incident->status);
		return;
	}
}

// function that shows all interventions in order
void show_all_interv(system_t *sys, FILE *fout)
{
	if (sys->interventions->size == 0) {
		fprintf(fout, "No intervention has been initiated\n");
		return;
	}

	intervention_t *interv = NULL;

	interv = sys->interventions->sentinel->next;
	while (interv != sys->interventions->sentinel) {
		fprintf(fout, "Incident %d was assigned to unit %d, "
			"and has the following status: \"%s\"\n",
			interv->incident->id, interv->unit->id,
			interv->incident->status);
		interv = interv->next;
	}
}

// core function, read inputs, initiqlizes units, executes all commands
void rule_all_commands(FILE *fin, FILE *fout, system_t *sistemul)
{
	char *command = calloc(MAX_LEN, sizeof(char));
	DIE(!command, "malloc command");

	fscanf(fin, "%u", &sistemul->units_nr);

	sistemul->units = malloc(sistemul->units_nr * sizeof(unit_t));
	DIE(!sistemul->units, "malloc units");

	for (unsigned int i = 0; i < sistemul->units_nr; i++) {
		fscanf(fin, "%d %c", &sistemul->units[i].id,
		       &sistemul->units[i].type);
		sistemul->units[i].availability = 1;
		enqueue(sistemul->queue_available_units,
			&sistemul->units[i]);
	}

	int nr_of_interv;

	fscanf(fin, "%d", &nr_of_interv);
	fgetc(fin);

	for (int i = 0; i < nr_of_interv; i++) {
		fgets(command, MAX_LEN, fin);

		if (strncmp(command, "ADD_INCIDENT", 12) == 0) {
			add_incident(sistemul, command);
		} else if (strncmp(command, "CHECK_UNITS", 11) == 0) {
			check_units(fout, sistemul);
		} else if (strncmp(command, "DISPATCH", 8) == 0) {
			dispatch(sistemul, fout);
		} else if (strncmp(command, "UNDO_LAST", 9) == 0) {
			undo_lst_dispatch(sistemul, fout);
		} else if (strncmp(command, "SOLVED", 6) == 0) {
			solved_incident(sistemul, command, fout);
		} else if (strncmp(command, "SHOW_UNIT", 9) == 0) {
			show_unit(sistemul, command, fout);
		} else if (strncmp(command, "SHOW_INCIDENT", 13) == 0) {
			show_incident(sistemul, command, fout);
		} else if (strncmp(command, "SHOW_INTERVENTIONS", 18) == 0) {
			show_all_interv(sistemul, fout);
		}
	}

	free(command);
}
