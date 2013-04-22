#ifndef _MAP_H_
#define _MAP_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef unsigned int key_t;
typedef struct file* value_t;

#define MAP_SIZE 256

struct map { value_t content[MAP_SIZE]; };

void map_init(struct map* m);
value_t map_find(struct map* m, key_t k);
key_t map_insert(struct map* m, value_t v);
value_t map_remove(struct map* m, key_t k);
void map_for_each(struct map* m,
                  void (*exec)(key_t k, value_t v, int aux),
                  int aux);
//void p_map_remove(struct p_map* m, int proc_id);
void map_remove_if(struct map* m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux);

bool close_helper(key_t k, value_t v, int aux);

#endif
