//
//  map_node.h
//  
//
//  Created by Niklas Blomqvist on 2013-02-26.
//
//

#ifndef _map_node_h
#define _map_node_h

#include <stdbool.h>

typedef void* value_t;
typedef unsigned int key_t;


struct map_node
{
  value_t value;
  key_t   key;
  struct map_node* next;
};

struct map_list
{
  struct map_node* node;
};

void map_init(struct map_node* m);
value_t map_find(struct map_node* m, key_t k);
key_t map_insert(struct map_node* m, value_t v);
value_t map_remove(struct map_node* m, key_t k);
void map_for_each(struct map_node* m,
                  void (*exec)(key_t k, value_t v, int aux),
                  int aux);
void map_remove_if(struct map_node* m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux);


#endif
