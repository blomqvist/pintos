#include <stdio.h>

#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

#include "userprog/process.h"
#include "userprog/plist.h"
#include "userprog/sys_proc_calls.h"

void sys_plist()
{
  printf("\nProcess\tParent\tAlive\tExit\tFree\tParent Alive\tName\n");
  process_print_list();
}

void sys_sleep(unsigned millis)
{
  printf("\nIn sleep(%d)\n", millis);
}

pid_t sys_exec(char* command_line_arg)
{
  return process_execute(command_line_arg);
}

void sys_exit(int status)
{
  process_exit(status);
  process_cleanup();
  sys_plist();
}
