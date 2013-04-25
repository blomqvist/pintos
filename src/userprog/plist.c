#include <stddef.h>

#include "userprog/plist.h"
#include "threads/malloc.h"

void p_map_init(struct p_map* m)
{
  unsigned it = 0;
  
  for(; it < PM_SIZE; ++it)
    m->content[it] = NULL;
}

p_value_t p_map_find(struct p_map* m, p_key_t k)
{
  unsigned it = 0;
  
  for(; it < PM_SIZE; ++it)
    if(m->content[it] != NULL && m->content[it]->proc_id == (int)k)
      return m->content[it];
  
  return NULL;
}

p_key_t p_map_insert(struct p_map* m, p_value_t v)
{
  unsigned it = 0;
  
  //Find first free element
  while(m->content[it] != NULL && it < PM_SIZE)
    ++it;
  
  //Is full
  if(it == PM_SIZE)
    return PM_SIZE;
  
  m->content[it] = v;
  
  //Return the index
  return it;
}

p_value_t p_map_remove(struct p_map* m, p_key_t k)
{
  unsigned it = 0;
  p_value_t ret_val = NULL;
  
  for(; it < PM_SIZE; ++it)
    if(m->content[it] != NULL && m->content[it]->proc_id == (int)k) {
      ret_val = m->content[it];
      
      free(m->content[it]->proc_name);
      free(m->content[it]);
      
      m->content[it] = NULL;
      
      break;
    }
  
  return ret_val;
}

void p_map_for_each(struct p_map* m,
		    void (*exec)(p_key_t k, p_value_t v, int aux),
		    int aux)
{
  unsigned it = 0;
  
  for(; it < PM_SIZE; ++it)
    if(m->content[it] != NULL)
      exec(it, m->content[it], aux);
}

void p_map_remove_if(struct p_map* m,
		     bool (*cond)(p_key_t k, p_value_t v, int aux),
		     int aux)
{
  unsigned it = 0;
  
  for(; it < PM_SIZE; ++it)
    if(m->content[it] != NULL)
      if(cond(it, m->content[it], aux))
	{
	  free(m->content[it]->proc_name);
	  free(m->content[it]);
      
	  m->content[it] = NULL;
	}
}
