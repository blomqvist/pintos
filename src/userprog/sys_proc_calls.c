#include <stdio.h>

#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

#include "userprog/process.h"
#include "userprog/plist.h"
#include "userprog/sys_proc_calls.h"

void sys_plist()
{
  printf("Process\tParent\tAlive\tExit\tFree\tT-ID");
  p_map_for_each(&p_map, plist_print_row, 0);
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

void plist_print_row(p_key_t k, p_value_t v, int aux)
{
  ++aux;
  printf("%d\t\t\t%d\t\t\t%d\t\t%d\t\t%d\t\t%d",
         v->proc_id, v->parent_id,
         v->alive, v->exit_status,
         v->free, k);
}
