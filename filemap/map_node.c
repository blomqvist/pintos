//
//  map_node.c
//  
//
//  Created by Niklas Blomqvist on 2013-02-26.
//
//

#include "map_node.h"
#include <stdlib.h>
#include <stdio.h>

// Initializes map with known default values
void map_init(struct map_node* m)
{
  //m = malloc(sizeof(struct map_node));
  m->key = 0;
  m->value = NULL;
  m->next = NULL;
}

value_t map_find(struct map_node* m, key_t k)
{
  if(m->key == k)
    return m->value;
  else if(m->next == NULL)
    return NULL;
  else
    return map_find(m->next, k);
}

key_t map_insert(struct map_node* m, value_t v)
{
  //printf("in map insert\n");
  //printf("m->value = 0x%p, m->next = 0x%p\n", m->value, m->next);
  if(m->value == NULL && m->next == NULL) { // Vi är sist i map:en
    
    //printf("setting m->value = v\n");
    m->value = v;
    
    // Allokera minne för nästa objekt i map:en
    m->next = malloc(sizeof(struct map_node));
    //printf("trying map_init(m->next)\n");
    map_init(m->next);
    m->next->key = m->key + 1;
    
    return m->key;
    // Returnera nuvarande index
  }
  else {
    // Vi är inte sist i listan, stega tills vi kommer dit
    return map_insert(m->next, v);
  }
}

value_t map_remove(struct map_node* m, key_t k)
{
  value_t temp_value = NULL;
  if(m->key == k) { // v finns i första elementet
    if (m->next == NULL) {  // första och enda elementet
      temp_value = m->value;
      free(m); // töm m i minnet
      
      m = malloc(sizeof(struct map_node));
      map_init(m); // ge standardvärden
    }
    else { // vi har fler följande element
      temp_value = m->value;
      struct map_node* temp = m;
      m = m->next; // första elementet blir efterföljande element
      free(temp); // frigör (det som var) första elementet
    }
    return temp_value;
  }
  // [x] -> [n]->key
  else if(m->next != NULL && m->next->key == k) {
    if (m->next->next == NULL) { // Vi är på sista elementet
      temp_value = m->next->value;
      free(m->next);
      m->next = NULL;
      m->next = malloc(sizeof(struct map_node));
    }
    else {
      temp_value = m->value;
      struct map_node* temp = m->next;
      m->next = m->next->next;
      free(temp);
    }
    return temp_value;
  }
  else if(m->next == NULL) // Vi är i slutet och v finns ej
    return temp_value;
  else // iterera vidare
    return map_remove(m->next, k);
}

void map_for_each(struct map_node* m,
                  void (*exec)(key_t k, value_t v, int aux),
                  int aux)
{
  if (m->value != NULL)
    exec(m->key, m->value, aux);
  if (m->next != NULL)
    map_for_each(m->next, exec, aux);
}

void map_remove_if(struct map_node* m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux)
{
  if (cond(m->key, m->value, aux) && m->value != NULL) // removes from map
    map_remove(m, m->key);
  
  if (m->next != NULL)
    map_remove_if(m->next, cond, aux);
}

