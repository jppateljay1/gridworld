/*
 * Jay Patel
 * file:// gw.h
 */
 
#ifndef GW_H_INCLUDED
#define GW_H_INCLUDED


// GW is a struct.  The actual contents of the
//   struct are implementation specific and hidden in the .c file (gw.c)
// Notice that all operations from the client's point of view are on GW pointers
typedef struct gw_struct GW;

extern GW *gw_build(int nrows, int ncols, int pop, int rnd);

extern int * gw_members(GW *g, int i, int j, int *n);

extern int gw_district_pop(GW *g, int i, int j);

extern int gw_total_pop(GW *g);

extern int gw_move(GW *g, int x, int i, int j);

extern int gw_find(GW *g, int x, int *r, int *c);

extern int gw_kill(GW *g, int x);

extern int gw_create(GW *g, int i, int j);

extern void gw_free(GW *g);


#endif // GW_H_INCLUDED
