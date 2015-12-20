/*
 * Jay Patel
 * file:// main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gw.h"

int main(int argc, char *argv[])
{	
	int nrows = 0, ncols = 0, pop = 0, rnd = 0;
	if(argc > 1)
	{
		int i;
		for(i = 1; i < argc; i++)
		{
			if(strcmp(argv[i], "-N") == 0)
			{
				i++;
				pop = atoi(argv[i]);
			}
			else if(strcmp(argv[i], "-R") == 0)
			{
				i++;
				nrows = atoi(argv[i]);
			}
			else if(strcmp(argv[i], "-C") == 0)
			{
				i++;
				ncols = atoi(argv[i]);
			}
			else if(strcmp(argv[i], "-rand") == 0)
				rnd = 1;
		}
	}

	GW *g = gw_build(nrows, ncols, pop, rnd);

	/*int i = 0, j = 0;
	printf("population: %d\n", gw_district_pop(g, i, j));

	int total = gw_total_pop(g);
	printf("total population: %d\n", total);

	print_population(g);

	printf("gw_kill: %d\n", gw_kill(g, 5));
	printf("gw_kill: %d\n", gw_kill(g, 7));
	printf("gw_kill: %d\n", gw_kill(g, 5));

	print_population(g);

	total = gw_total_pop(g);
	printf("total population: %d\n", total);

	printf("Move: %d\n", gw_move(g, 5, 1, 4));

	printf("gw_create: %d\n", gw_create(g, 1, 4));
	printf("gw_create: %d\n", gw_create(g, 1, 4));
	printf("gw_create: %d\n", gw_create(g, 1, 4));
	print_deadlist(g);*/

	printf("\n\n\n");
	printf("-----------------------------------------------\n");
	printf("Enter a command or press ? for options\n");
	char word[30];
	int val1, val2, val3, val4, r, c;
	int *arr;

	while(1)
	{
		printf("-------------------------------------------\n");
		gets(word);

		if(strcmp(word, "quit") == 0)
			break;
		else if(strcmp(word, "?") == 0)
		{
			printf("members i j\n");
			printf("population i j\n");
			printf("population\n");
			printf("move x i j\n");
			printf("find x\n");
			printf("kill x\n");
			printf("create i j\n");
		}
		else if(strcmp(word, "population") == 0)
			printf("total population: %d\n", gw_total_pop(g));
		else
		{
			char temp[20];
			if(word[0] == 'p')
			{
				sscanf(word, "%s %d %d", temp, &val1, &val2);
				printf("population: %d\n", gw_district_pop(g, val1, val2));
			}
			else if(word[0] == 'm' && word[1] == 'e')
			{
				sscanf(word,"%s %d %d", temp, &val1, &val2);
				arr = gw_members(g, val1, val2, &val4);
			}
			else if(word[0] == 'm' && word[1] == 'o')
			{
				sscanf(word, "%s %d %d", temp, &val1, &val2);
				printf("move: %d\n", gw_move(g, val1, val2, val3));
			}
			else if(word[0] == 'f')
			{
				sscanf(word, "%s %d", temp, &val1);
				printf("find: %d\n", gw_find(g, val1, &r, &c));
			}
			else if(word[0] == 'k')
			{
				sscanf(word, "%s %d", temp, &val1);
				printf("kill: %d\n", gw_kill(g, val1));
			}
			else if(word[0] == 'c')
			{
				sscanf(word, "%s %d %d", temp, &val1, &val2);
				printf("create: %d\n", gw_create(g, val1, val2));
			}
			else
				printf("Invalid entry, enter again\n");
		}
		printf("-------------------------------------------\n");
	}

	gw_free(g);

	return 0;
}












