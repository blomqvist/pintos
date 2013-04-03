//
//  stack_helper.h
//  
//
//  Created by Niklas Blomqvist on 2013-02-25.
//
//

#ifndef _stack_helper_h
#define _stack_helper_h

struct main_args
{
  /* Hint: When try to interpret C-declarations, read from right to
   * left! It is often easier to get the correct interpretation,
   * altough it does not always work. */
  
  /* Variable "ret" that stores address (*ret) to a function taking no
   * parameters (void) and returning nothing. */
  void (*ret)(void);
  
  /* Just a normal integer. */
  int argc;
  
  /* Variable "argv" that stores address to an address storing char.
   * That is: argv is a pointer to char*
   */
  char** argv;
};

bool exists_in(char c, const char d[]);
int count_args(const char buf[], const char delimeters[]);

void* setup_main_stack(const char* command_line, void* stack_top);


#endif
