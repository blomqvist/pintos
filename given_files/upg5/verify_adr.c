#include <stdlib.h>
#include "pagedir.h"


/* Kontrollera alla adresser från och med start till och inte med
 * (start+length). */
bool verify_fix_length(void* start, int length)
{
  // ADD YOUR CODE HERE
}

/* Kontrollera alla adresser från och med start till och med den
 * adress som först inehåller ett noll-tecken, `\0'. (C-strängar
 * lagras på detta sätt.) */
bool verify_variable_length(char* start)
{
  // ADD YOUR CODE HERE
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
