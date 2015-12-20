/*
 * Jay Patel
 * file:// gw.c
 */

#include "gw.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct dead_people
{
	int id;
	struct dead_people *next;
}DEAD_PEOPLE;

typedef struct person_pointer
{
	int id;
	int alive_status;
	int x;
	int y;
	struct person_pointer *next;
	struct person_pointer *prev;
}PERSON_POINTER;

typedef struct district_pointer
{
	int size;
	struct person_pointer *first;
}DISTRICT_POINTER;

struct gw_struct
{
	int ncol;
	int nrow;
	int pop;
	int total_population;
	struct person_pointer *person;
	struct district_pointer *district;
	DEAD_PEOPLE *dead_list;
};

//////////////////////////////HELPER FUNCTIONS FOR INTERNAL TESTING////////////////////////////////////
int return_ncols(GW *g)
{
	return g->ncol;
}

int return_nrows(GW *g)
{
	return g->nrow;
}

void print_population(GW *g)
{
	int i, j;
	int nrows = g->nrow;
	int ncols = g->ncol;

	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			printf("population: %d\n", gw_district_pop(g, i, j));
		}
	}
}

void print_deadlist(GW *g)
{
	DEAD_PEOPLE *p = g->dead_list;

	while(p != NULL)
	{
		printf("dead_list id: %d\n", p->id);
		p = p->next;
	}
}

////////////////////////////////////////END OF HELPER FUNCTINS//////////////////////////////

DISTRICT_POINTER *allocate_district(int size)
{
	DISTRICT_POINTER *dp = (DISTRICT_POINTER *)malloc(sizeof(DISTRICT_POINTER) * size);

	int i;
	for(i = 0; i < size; i++)
	{
		dp[i].size = 0;
		dp[i].first = NULL;
	}

	return dp;
}

PERSON_POINTER *allocate_person(int size)
{
	PERSON_POINTER *pp = (PERSON_POINTER *)malloc(sizeof(PERSON_POINTER) * size);

	int i, j = 0, k = 0;
	for(i = 0; i < size; i++)
	{
		pp[i].id = i;
		pp[i].alive_status = 1;
		pp[i].next = NULL;
		pp[i].prev = NULL;
	}

	return pp;
}

DEAD_PEOPLE *allocate_deadlist()
{
	DEAD_PEOPLE *dp = malloc(sizeof(DEAD_PEOPLE));
	dp->id = -999;
	dp->next = NULL;
	return dp;
}

void push_front(DISTRICT_POINTER *dp, PERSON_POINTER *pp)
{
	if(dp->first == NULL)
	{
		dp->first = pp;
		return;
	}
	PERSON_POINTER *temp = dp->first;
	temp->prev = pp;
	pp->next = temp;
	pp->prev = NULL;
	dp->first = pp;
}

/*
 * pushes the values on the stack of DEAD_PEOPLE 
 * uses the traditional push function
 * first come last out
 */
void push_front_dead(DEAD_PEOPLE **dp, int x)
{
	DEAD_PEOPLE *head = malloc(sizeof(DEAD_PEOPLE));
	head->id = x;
	head->next = NULL;

	if(dp == NULL)
	{
		dp = &head;
		return;
	}

	head->next = *dp;
	*dp = head;

}

/*
 * pop's the first element of the DEAD_PEOPLE
 */
int pop_front_deadlist(DEAD_PEOPLE **dp)
{
	/*
	 * although the if statement will never be true
	 * it is just there as a safety precausion
	 */
	if(dp == NULL)
		return -999;

	DEAD_PEOPLE *temp = *dp;
	int ret_val = temp->id;
	*dp = temp->next;
	free(temp);

	return ret_val;
}

/*
 * frees the district array one element at a time
 */
void free_district(DISTRICT_POINTER *dp, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		if(dp[i].first == NULL)
			continue;
		free(&dp[i]);
	}
}

/*
 * makes the grid grow by one person
 * as the function gets called from gw_create
 * 
 * creates an empty list of PERSON_POINTER temp
 * copies the list from g->person into temp
 * frees the old g->person
 * re-assigns g->person <- temp
 */
void expand_grid(GW *g)
{
	int n = g->pop;

	PERSON_POINTER *pp = allocate_person(n);
	PERSON_POINTER *tmp = g->person;

	int i;
	for(i = 0; i < n; i++)
	{
		pp[i].id = tmp[i].id;
		pp[i].alive_status = tmp[i].alive_status;
		pp[i].x = tmp[i].x;
		pp[i].y = tmp[i].y;
		pp[i].next = tmp[i].next;
		pp[i].prev = tmp[i].prev;
	}

	g->person = pp;
	free(tmp);
}

GW *gw_build(int nrows, int ncols, int pop, int rnd)
{
	if(nrows < 1)
		nrows = 5;
	if(ncols < 1)
		ncols = 5;
	if(pop < 1)
		pop = 10;

	int temp = nrows * ncols;
	GW *g = (GW *)malloc(sizeof(GW));

	g->nrow = nrows;
	g->ncol = ncols;
	g->pop = pop;
	g->total_population = pop;
	g->district = allocate_district(temp);
	g->person = allocate_person(pop);
	g->dead_list = NULL;

	int i;
	for(i = 0; i < pop; i++)
	{
		int r = 0;
		if(rnd == 1)
			r = rand() % temp;
		int tempx, tempy;
		tempx = r % nrows;
		tempy = r - (tempx * nrows);
		g->person[i].x = tempx;
		g->person[i].y = tempy;
		push_front(&(g->district[r]), &(g->person[i]));
		(g->district[r].size)++;
	}

	return g;
}

int *gw_members(GW *g, int i, int j, int *n)
{
	*n = 0;
	int size = g->district[i * g->ncol + j].size;
	int *arr = (int *)malloc(sizeof(int) * size);
	PERSON_POINTER *p = g->district[i * g->ncol + j].first;

	int k = 0, temp;

	while(p != NULL)
	{
		temp = p->id;
		printf("%d, ", temp);
		arr[k] = temp;
		(*n)++;
		p = p->next;
	}

	printf("\n");

	return arr;
}

int gw_district_pop(GW *g, int i, int j)
{
	if(i > g->nrow || j > g->ncol)
		return -1;
	return g->district[i * g->ncol + j].size;
}

int gw_total_pop(GW *g)
{
	return g->pop;
}

int gw_move(GW *g, int x, int i, int j)
{
	if(g->person[x].alive_status == 0)
		return 0;

	int pos_x = g->person[x].x;
	int pos_y = g->person[x].y;

	if(g->district[pos_x * g->ncol + pos_y].size == 1)
	{
		g->district[pos_x * g->ncol + pos_y].first = NULL;
		g->district[pos_x * g->ncol + pos_y].size = 0;
		push_front(&(g->district[i * g->ncol + j]), &(g->person[x]));
		(g->district[i * g->ncol + j].size)++;

		return 1;
	}

	PERSON_POINTER *prev_p = g->person[x].prev;

	g->person[x].prev->next = g->person[x].next;
	g->person[x].next->prev = prev_p;
	(g->district[pos_x * g->ncol + pos_y].size)--;
	push_front(&(g->district[i * g->ncol + j]), &(g->person[x]));
	(g->district[i * g->ncol + j].size)++;

	return 1;
}

int gw_find(GW *g, int x, int *r, int *c)
{
	if(g->person[x].alive_status == 0)
		return 0;
	*r = g->person[x].x;
	*c = g->person[x].y;

	return 1;
}

int gw_kill(GW *g, int x)
{
	if(g->person[x].alive_status == 0)
		return 0;
	int pos_x = g->person[x].x;
	int pos_y = g->person[x].y;

	if(x == 0)
	{
		PERSON_POINTER *prev_p = g->person[x].prev;
		g->person[x].prev->next = g->person[x].prev;
		(g->district[pos_x * g->ncol + pos_y].size)--;
		g->person[x].alive_status = 0;
		(g->pop)--;

		push_front_dead(&g->dead_list, x);
		//push_front_dead(g->dead_list, x);

		return 1;
	}

	if(g->district[pos_x * g->ncol + pos_y].size == 1)
	{
		g->district[pos_x * g->ncol + pos_y].first = NULL;
		(g->district[pos_x * g->ncol + pos_y].size)--;
		g->person[x].alive_status = 0;
		(g->pop)--;

		push_front_dead(&g->dead_list, x);
		//push_front_dead(g->dead_list, x);

		return 1;
	}

	PERSON_POINTER *prev_p = g->person[x].prev;

	g->person[x].prev->next = g->person[x].prev;
	g->person[x].next->prev = prev_p;
	(g->district[pos_x * g->ncol + pos_y].size)--;
	g->person[x].alive_status = 0;
	(g->pop)--;

	push_front_dead(&g->dead_list, x);
	//push_front_dead(g->dead_list, x);

	return 1;
}

int gw_create(GW *g, int i, int j)
{
	if(i > g->nrow || j > g->ncol)
		return -1;

	int id;
	if(g->total_population > g->pop)
	{
		id = pop_front_deadlist(&g->dead_list);

		/*
		 * this will never be the case
		 * but putting it just for safety measures
		 */
		if(id == -999)
		{
			(g->pop)++;
			(g->total_population)++;
			expand_grid(g);
			id = g->pop;
		}
	}
	else
	{
		(g->pop)++;
		(g->total_population)++;
		expand_grid(g);
		id = g->pop;
		printf("id: %d\n", id);
		printf("pop: %d\n", g->pop);
	}

	g->person[id].alive_status = 1;
	g->person[id].x = i;
	g->person[id].y = j;

	push_front(&(g->district[i * g->ncol + j]), &(g->person[i]));

	(g->district[i * g->ncol + j].size)++;

	return id;
}

void gw_free(GW *g)
{
	int n = g->ncol * g->nrow;
	free_district(g->district, n-1);
	free(g->person);
}
