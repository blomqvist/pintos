#include "threads/malloc.h"
#include "filesys/filesys.h"
#include <stddef.h>
#include <stdio.h>
#include "flist.h"

/*
 * Fixa bättre kommentarer
 */

// Initializes map with known default values
void map_init(struct map* m)
{
  m->key = 3;
  m->value = NULL;
  m->next = NULL;
}

value_t map_find(struct map* m, key_t k)
{
  struct map* temp = m;
  for (;;)
  {
    if (temp->value != NULL && temp->key == k)
    {
      return temp->value;
    }
    
    if (temp->next == NULL)
      return NULL;
    
    temp = temp->next;
  }
}

key_t map_insert(struct map* m, value_t v)
{
  struct map* temp = m;
  for (;;)
  {
    if (temp->value == NULL && temp->next == NULL)
    {
      temp->value = v;
      
      // Allokera minne för nästa objekt
      temp->next = (void*)malloc(sizeof(struct map));
      map_init(temp->next);
      temp->next->key = temp->key + 1;
      
      // Returnera index
      return temp->key;
    }
    else
    {
      // Stega vidare
      temp = temp->next;
    }
  }
}

value_t map_remove(struct map* m, key_t k)
{
  struct map* curr = m;
  struct map* prev = NULL;
  struct map* temp = m;
  value_t ret = NULL;
  
  while (curr != NULL)
  {
    if (curr->key != k)
    {
      prev = curr;
      curr = curr->next;
    }
    else
    {
      if (curr->next != NULL)
        temp = curr->next;
      
      if (prev != NULL)
        prev->next = temp;
      
      if (curr != m)
      {
        ret = curr->value;
        free (curr);
        curr = temp;
      }
      else
        map_init(m);
    }
  }
  
  return ret;
}

void map_for_each(struct map* m,
                  void (*exec)(key_t k, value_t v, int aux),
                  int aux)
{
  struct map* temp = m;
  while (temp != NULL)
  {
    exec(temp->key, temp->value, aux);
    if (temp->next == NULL)
      break;
    temp = temp->next;
  }
}

void map_remove_if(struct map* m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux)
{
  struct map* curr = m;
  struct map* prev = NULL;
  struct map* temp = m;
  
  while (curr != NULL)
  {
    if (!cond(curr->key, curr->value, aux))
    {
      prev = curr;
      curr = curr->next;
    }
    else
    {
      if (curr->next != NULL)
        temp = curr->next;
      
      if (prev != NULL)
        prev->next = temp;
      
      if (curr != m)
      {
        free (curr);
        curr = temp;
      }
      else
        map_init(m);
    }
  }
}

bool close_helper(key_t k UNUSED, value_t v, int aux UNUSED)
{
  filesys_close(v);
  return true;
}



