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
}

void p_map_for_each(struct p_map* m,
                  void (*exec)(p_key_t k, p_value_t v, int aux),
                  int aux)
{
  struct p_map* temp = m;
  while (temp->value != NULL)
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
  struct p_map* temp = m;
  struct p_map* temp_free = NULL;
  /**
   * Specialfall för första element
   **/
  if (cond(m->key, m->value, aux)) // Vi står i första elementet
  {
    if (m->next->value == NULL) // Inga efterföljande element
    {
      p_map_init(m);
    }
    else // Vi har efterföljande element
    {
      temp = m;
      m = m->next;
      free(temp);
    }
    return; // Vi är färdiga
  }
  
  while (temp != NULL)
  {
    if (cond(temp->next->key, temp->next->value, aux))
    {
      /**
       * [1] -> [2] -> [3]
       * [1] -> [3]
       * free([2])
       **/
      temp_free = temp->next;
      temp->next = temp->next->next;
      free(temp_free); // free([2])
    }
    
    if (temp->next != NULL)
      return;
    
    temp = temp->next;
  }
}



