#ifndef _PLIST_H_
#define _PLIST_H_

#include <stdbool.h>
#include <stdlib.h>
#include "threads/synch.h"

typedef int pid_t;

/**
 * Processinfo-struct
 * wait är mest för debug
 **/
struct proc_table
{
  bool alive;                 // Lever den
  bool free;                  // Se om processen är klar
  bool parent_alive;          // Lever föräldern?
  pid_t proc_id;              // Process-ID
  pid_t parent_id;            // Förälder-ID
  pid_t wait;                 // Vem väntar vi på?
  int exit_status;            // Exit status
  struct semaphore semaphore; // Semaphore
  char* proc_name;            // Process name
};

typedef unsigned int p_key_t;
typedef struct proc_table* p_value_t;

struct p_map
{
  size_t key;
  p_value_t value;
  
  struct p_map* next; // next element
};


struct p_map p_map; // INITIERING KÄN*S GALETS!! HEH :M

void p_map_init(struct p_map* m);
p_value_t p_map_find(struct p_map* m, p_key_t k);
p_key_t p_map_insert(struct p_map* m, p_value_t v);
void p_map_for_each(struct p_map* m,
                  void (*exec)(p_key_t k, p_value_t v, int aux),
                  int aux);
void p_map_remove_if(struct p_map* m,
                   bool (*cond)(p_key_t k, p_value_t v, int aux),
                   int aux);

/* Initiering av plist map */
struct p_map p_map;

#endif
