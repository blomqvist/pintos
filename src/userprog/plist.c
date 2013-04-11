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

p_value_t p_map_remove(struct p_map* m, p_key_t k)
{
  p_value_t temp_value = NULL;
  if(m->key == k) { // v finns i första elementet
    if (m->next == NULL) {  // första och enda elementet
      temp_value = m->value;
      //free(m); // töm m i minnet
      //m = (void*)malloc(sizeof(struct p_map));
      p_map_init(m); // ge standardvärden
    }
    else { // vi har fler följande element
      temp_value = m->value;
      struct p_map* temp = m;
      m = m->next; // första elementet blir efterföljande element
      temp = NULL;
      //p_map_init(m); // frigör (det som var) första elementet
    }
    
    return temp_value;
  }
  // [x] -> [n]->key
  else if(m->next != NULL && m->next->key == k) {
    if (m->next->next == NULL) { // Vi är på sista elementet
      temp_value = m->next->value;
      free(m->next);
      m->next = NULL;
      m->next = malloc(sizeof(struct p_map));
    }
    else {
      temp_value = m->value;
      struct p_map* temp = m->next;
      m->next = m->next->next;
      temp = NULL;//free(temp);
    }
    return temp_value;
  }
  else if(m->next == NULL) // Vi är i slutet och v finns ej
    return temp_value;
  else // iterera vidare
    return p_map_remove(m->next, k);
}

void p_map_for_each(struct p_map* m,
                  void (*exec)(p_key_t k, p_value_t v, int aux),
                  int aux)
{
  exec(m->key, m->value, aux);
  if (m->next != NULL && m->next->value != NULL)
    p_map_for_each(m->next, exec, aux);
}

void p_map_remove_if(struct p_map* m,
                   bool (*cond)(p_key_t k, p_value_t v, int aux),
                   int aux)
{
  if (cond(m->key, m->value, aux)) // removes from p_map
    p_map_remove(m, m->key);
  
  if (m->next != NULL && m->next->value != NULL)
    p_map_remove_if(m->next, cond, aux);
}



