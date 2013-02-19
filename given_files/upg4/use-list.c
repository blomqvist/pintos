#include <stdlib.h>
#include <stdio.h>

#include "list/list.h"

int main()
{
  int i, j;
  int N = 1000;

  /* create a list */
  for (i = 2; i < N; ++i)
  {
    /* insert the value of the loop variable 'i' in the list */
  }
  
  for (i = 2; i < N; ++i)
  {
    for (j = i*2; j < N; j += i)
    {
      /* remove every with 'j' even divisible number from the list */
    }
  }
  
  /* print all remaining numbers in the list and free any memory
   * allocated */
  return 0;
}
