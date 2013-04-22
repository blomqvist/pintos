#include "threads/malloc.h"
#include "filesys/filesys.h"

#include "flist.h"

void map_init(struct map* m)
{
  unsigned it = 0;
  
  for(; it < MAP_SIZE; ++it)
    m->content[it] = NULL;
}

value_t map_find(struct map* m, key_t k)
{
  return m->content[k];
}

key_t map_insert(struct map* m, value_t v)
{
  unsigned it = 3;
  
  while(m->content[it] != NULL && it < MAP_SIZE)
    ++it;
    
  if(it == MAP_SIZE)
    return MAP_SIZE;
  
  m->content[it] = v;
  
  return it;
}

value_t map_remove(struct map* m, key_t k)
{
  value_t ret_val = m->content[k];
  m->content[k] = NULL;
  
  return ret_val;
}

void map_for_each(struct map* m,
		  void (*exec)(key_t k, value_t v, int aux),
		  int aux)
{
  unsigned it = 3;
  
  for(; it < MAP_SIZE; ++it)
    if(m->content[it] != NULL)
      exec(it, m->content[it], aux);
}

void map_remove_if(struct map* m,
		   bool (*cond)(key_t k, value_t v, int aux),
		   int aux)
{
  unsigned it = 3;
  
  for(; it < MAP_SIZE; ++it)
    if(cond(it, m->content[it], aux))
      map_remove(m, it);
}

bool close_helper(key_t k UNUSED, value_t v, int aux UNUSED)
{
  filesys_close(v);
  
  return true;
}
