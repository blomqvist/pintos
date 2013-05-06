#ifndef _PLIST_H
#define _PLIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "threads/synch.h"

#define PM_SIZE 256

typedef int pid_t;
typedef unsigned p_key_t;
typedef struct proc_table* p_value_t;

struct proc_table
{
  bool alive;                 // Lever den
  bool parent_alive;          // Lever föräldern?
  pid_t proc_id;              // Process-ID
  pid_t parent_id;            // Förälder-ID
  pid_t wait;                 // Vem väntar vi på?
  int exit_status;            // Exit status
  struct semaphore semaphore; // Semaphore
  char* proc_name;            // Process name
};

struct p_map
{ 
  p_value_t content[PM_SIZE];
  
  struct lock plock;
};

/* Declaration of global p_map */
struct p_map p_map;

void p_map_init(struct p_map* m);
p_value_t p_map_find(struct p_map* m, p_key_t k);
p_key_t p_map_insert(struct p_map* m, p_value_t v);
p_value_t p_map_remove(struct p_map* m, p_key_t k);
void p_map_for_each(struct p_map* m,
		    void (*exec)(p_key_t k, p_value_t v, int aux),
		    int aux);
void p_map_remove_if(struct p_map* m,
		     bool (*cond)(p_key_t k, p_value_t v, int aux),
		     int aux);

#endif
