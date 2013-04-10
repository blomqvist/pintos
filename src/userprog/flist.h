#ifndef _MAP_H_
#define _MAP_H_

#include <stdbool.h>
#include <stdlib.h>

typedef unsigned int key_t;
typedef struct file* value_t;

struct map
{
  size_t key;
  value_t value;
  
  struct map* next; // next element
};

struct map_list
{
  struct map* node;
};

void map_init(struct map* m);
value_t map_find(struct map* m, key_t k);
key_t map_insert(struct map* m, value_t v);
value_t map_remove(struct map* m, key_t k);
void map_for_each(struct map* m,
                  void (*exec)(key_t k, value_t v, int aux),
                  int aux);
void map_remove_if(struct map* m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux);

bool close_helper(key_t k, value_t v, int aux);

#endif
