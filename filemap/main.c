//
//  main.c
//  
//
//  Created by Niklas Blomqvist on 2013-02-26.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "map_node.h"

void print_map (key_t k, value_t v, int aux)
{
  printf("(%d) %d => %c\n", aux, k, (char)(int)v);
}

int main()
{
  struct map_list list;
  list.node = malloc(sizeof(struct map_node));
  map_init(list.node);

  for (int i = 0; i < 50; i++)
  {
    char* temp = (char*)(i + 48);
    map_insert(list.node, (void*)temp);
  }
  
  map_for_each(list.node, print_map, 0);
  
  for (int i = 0; i < 100; i++)
    map_remove(list.node, i);
  
  map_for_each(list.node, print_map, 1);
  
  
  return 0;
}
