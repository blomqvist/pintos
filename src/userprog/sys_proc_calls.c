#include <stdio.h>

#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

#include "devices/timer.h"

#include "userprog/process.h"
#include "userprog/plist.h"
#include "userprog/sys_proc_calls.h"

void sys_plist()
{
  printf("\nProcess\tParent\tAlive\tExit\tParent Alive\tName\n");
  process_print_list();
}

void sys_sleep(unsigned millis)
{
  timer_msleep((int64_t)millis);
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

int
sys_wait(pid_t id)
{
  return process_wait((pid_t)id);
}
