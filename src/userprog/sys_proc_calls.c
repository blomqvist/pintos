#include <stdio.h>

#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

#include "userprog/process.h"
#include "userprog/plist.h"
#include "userprog/sys_proc_calls.h"

void sys_plist()
{
  printf("\nProcess\tParent\tAlive\tExit\tFree\tT-ID\n");
}

void sys_sleep(unsigned millis)
{
  printf("\nIn sleep(%d)\n", millis);
  
}

pid_t sys_exec(char* command_line_arg)
{
  pid_t process_id;
  process_id = process_execute(command_line_arg);
  
  if (process_id != -1)
  {
    struct proc_table p_table;
    p_table.proc_id = process_id;
    sema_init(&p_table.semaphore, 1);
    process_id = p_map_insert(&p_map, &p_table);
  }
  
  printf("\ZERG LOL: %d\n", p_map.value->proc_id);
  return process_id; // Ändra!
}
