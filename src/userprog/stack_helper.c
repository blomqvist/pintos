//
//  stack_helper.c
//  
//
//  Created by Niklas Blomqvist on 2013-02-25.
//
//

#include <stdbool.h>
#include "stack_helper.h"
#include "lib/string.h"

#define STACK_DEBUG(...) 

/* Return true if 'c' is fount in the c-string 'd'
 */
bool exists_in(char c, const char d[])
{
  int i = 0;
  while (d[i] != '\0' && d[i] != c)
    ++i;
  return (d[i] == c);
}

/* Return the number of words in 'buf'. A word is defined as a
 * sequence of characters not containing any of the characters in
 * 'delimeters'.
 */
int count_args(const char buf[], const char delimeters[])
{
  int i = 0;
  bool prev_was_delim;
  bool cur_is_delim = true;
  int argc = 0;
  
  while (buf[i] != '\0')
  {
    prev_was_delim = cur_is_delim;
    cur_is_delim = exists_in(buf[i], delimeters);
    argc += (prev_was_delim && !cur_is_delim);
    ++i;
  }
  return argc;
}

void* setup_main_stack(const char* command_line, void* stack_top)
{
  /* Variable "esp" stores an address, and at the memory loaction
   * pointed out by that address a "struct main_args" is found.
   * That is: "esp" is a pointer to "struct main_args" */
  struct main_args* esp;
  int argc = 0;
  int total_size;
  int line_size;
  /* "cmd_line_on_stack" and "ptr_save" are variables that each store
   * one address, and at that address (the first) char (of a possible
   * sequence) can be found. */
  char* cmd_line_on_stack;
  char* ptr_save;
  
  /* calculate the bytes needed to store the command_line */
  line_size = strlen(command_line);
  STACK_DEBUG("# line_size = %d\n", line_size);
  
  /* round up to make it even divisible by 4 */
  unsigned div_rest = line_size % 4;
  if(div_rest > 0)
    line_size += 4 - div_rest;
  STACK_DEBUG("# line_size (aligned) = %d\n", line_size);
  
  /* calculate how many words the command_line contains */
  argc = count_args(command_line, " ");
  STACK_DEBUG("# argc = %d\n", argc);
  
  /* calculate the size needed on our simulated stack */
  total_size = 12 + (4 * argc) + line_size;
  STACK_DEBUG("# total_size = %d\n", total_size);
  
  /* calculate where the final stack top will be located */
  esp = (void*)((int)stack_top - total_size - 4);
  
  /* setup return address and argument count */
  esp->ret = 0x00;
  esp->argc = argc;
  /* calculate where in the memory the argv array starts */
  esp->argv = (char**)((int)esp + 12);
  
  /* calculate where in the memory the words are stored */
  cmd_line_on_stack = (char*)((int)esp + (total_size - line_size) + 4);
  
  /* copy the command_line to where it should be in the stack */
  //*cmd_line_on_stack = command_line;
  
  //printf("cmd_line_on_stack = %s\n", cmd_line_on_stack);
  //exit(0);
  /* build argv array and insert null-characters after each word */
  char* temp_str = (char*)command_line;
  char* token;
  int i = 0;
  
  for(token = strtok_r(temp_str, " ", &ptr_save);
      token != NULL;
      token = strtok_r(NULL, " ", &ptr_save))
  {
    int token_len = strlen(token);
    strlcpy(cmd_line_on_stack, token, token_len + 1);
    esp->argv[i] = cmd_line_on_stack;
    cmd_line_on_stack += token_len;
    *cmd_line_on_stack++ = '\0';
    //esp->argv[++i] = NULL;
    i++;
  }
  //*(esp->argv[i]) = NULL;
  
  return esp; /* the new stack top */
}



