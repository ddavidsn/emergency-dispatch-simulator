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

// very short main, opens input and output files, initializes the system
// executes all commands with the made function, and at the end
// frees the necessary memory
int main(void)
{
	FILE *fin = fopen("tema1.in", "r");
	DIE(fin == NULL, "fopen fin tema1.in");
	FILE *fout = fopen("tema1.out", "w");
	DIE(fout == NULL, "fopen fout tema1.out");

	system_t *sistemul = system_create();

	rule_all_commands(fin, fout, sistemul);

	free_system(sistemul);

	fclose(fin);
	fclose(fout);

	return 0;
}
