/* Nicolescu David-Stefan - 313CA */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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

#define MAX_LEN 4096

// all functions used in fucntions.c
void print_error_message(FILE *fout);
void add_incident(system_t *sys, char *comanda);
void check_units(FILE *fout, system_t *sys);
void dispatch(system_t *sys, FILE *fout);
void undo_lst_dispatch(system_t *sys, FILE *fout);
void solved_incident(system_t *sys, char *command, FILE *fout);
void show_unit(system_t *sys, char *command, FILE *fout);
void show_incident(system_t *sys, char *command, FILE *fout);
void show_all_interv(system_t *sys, FILE *fout);
void rule_all_commands(FILE *fin, FILE *fout, system_t *sistemul);

#endif
