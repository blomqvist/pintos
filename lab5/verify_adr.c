#include <stdlib.h>
#include "pagedir.h"


/**
 PAGE_NUMBER = USER_ADR / PAGE_SIZE; // heltalsdivision
 PAGE_OFFSET = USER_ADR % PAGE_SIZE; // resten vid divisionen
 FRAME_NUMBER = PAGETABLE[PAGE_NUMBER];
 PHYS_ADR = FRAME_NUMBER + PAGE_OFFSET;
 */

/* Kontrollera alla adresser från och med start till och inte med
 * (start+length). */
bool verify_fix_length(void* start, int length)
{
  long stop_addr = (long)start + length - 1;
  long temp_addr = (long)pg_round_down(start);
  for (; temp_addr <= stop_addr; temp_addr += PGSIZE)
  {
    if (pagedir_get_page(NULL, (void*)temp_addr) == NULL)
      return false;
  }
  return true;
}

/* Kontrollera alla adresser frŒn och med start till och med den
 * adress som fšrst innehŒller ett noll-tecken, `\0'. (C-strŠngar
 * lagras pŒ detta sŠtt.) */

bool verify_variable_length(char* start)
{
  unsigned prev_page;
  
  if (pagedir_get_page(NULL, start) == NULL)
  {
    return false;
  }
  else
  {
    prev_page = pg_no(start);
  }
  
  for (;;)
  {
    if (prev_page != pg_no(start))
    {
      if (pagedir_get_page(NULL, start) == NULL)
      {
        return false;
      }
      prev_page = pg_no(start);
    }
    
    if (is_end_of_string(start))
    {
      return true;
    }
    start++;
  }
}

/* Definition av testfall */
struct test_case_t
{
  void* start;
  unsigned length;
};

#define TEST_CASE_COUNT 6

const struct test_case_t test_case[TEST_CASE_COUNT] =
{
  {(void*)100, 100}, /* one full page */
  {(void*)199, 102},
  {(void*)101, 98},
  {(void*)250, 190},
  {(void*)250, 200},
  {(void*)250, 210}
};

/* Huvudprogrammet utvärderar din lösning. */
int main()
{
  int i;
  bool result;
  
  for (i = 0; i < TEST_CASE_COUNT; ++i)
  {
    /* Testa algoritmen med ett givet intervall (en buffer). */
    start_evaluate_algorithm(test_case[i].start, test_case[i].length);
    result = verify_fix_length(test_case[i].start, test_case[i].length);
    evaluate(result);
    end_evaluate_algorithm();
    
    /* Testa algoritmen med en sträng. */
    start_evaluate_algorithm(test_case[i].start, test_case[i].length);
    result = verify_variable_length(test_case[i].start);
    evaluate(result);
    end_evaluate_algorithm();
  }
  return 0;
}
