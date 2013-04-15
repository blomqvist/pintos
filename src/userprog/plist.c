#include <stddef.h>

#include "userprog/plist.h"
#include "threads/malloc.h"

// Initializes p_map with known default values
void p_map_init(struct p_map* m)
{
  m->key = 3;
  m->value = NULL;
  m->next = NULL;
}

p_value_t p_map_find(struct p_map* m, p_key_t k)
{
  if(m->key == k)
    return m->value;
  else if(m->next == NULL)
    return NULL;
  else
    return p_map_find(m->next, k);
}

p_key_t p_map_insert(struct p_map* m, p_value_t v)
{
  struct p_map* temp = m;
  for (;;)
  {
    if (temp->value == NULL && temp->next == NULL)
    {
      temp->value = v;
      
      // Allokera minne för nästa objekt
      temp->next = (void*)malloc(sizeof(struct p_map));
      p_map_init(temp->next);
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
  /**
   * Old map_insert
  if(m->value == NULL && m->next == NULL) { // Vi är sist i p_map:en
    m->value = v;
    
    // Allokera minne för nästa objekt i p_map:en
    m->next = (void*)malloc(sizeof(struct p_map));
    p_map_init(m->next);
    m->next->key = m->key + 1;
    
    return m->key;
    // Returnera nuvarande index
  }
  else {
    // Vi är inte sist i listan, stega tills vi kommer dit
    return p_map_insert(m->next, v);
  }
  **/
}

void p_map_for_each(struct p_map* m,
                  void (*exec)(p_key_t k, p_value_t v, int aux),
                  int aux)
{
  struct p_map* temp = m;
  while (temp != NULL)
  {
    exec(temp->key, temp->value, aux);
    if (temp->next == NULL)
      break;
    temp = temp->next;
  }
}

void p_map_remove_if(struct p_map* m,
                   bool (*cond)(p_key_t k, p_value_t v, int aux),
                   int aux)
{
  /**
   * 2000 % WORKIN, BITCHES!!
   **/
  struct p_map* curr = m;
  struct p_map* prev = NULL;
  struct p_map* temp = m;

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
  
      free (curr);
      curr = temp;
    }
  }
}



