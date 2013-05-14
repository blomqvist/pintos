#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

/* header files you probably need, they are not used yet */
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "devices/input.h"

/* our own sys call-files */
#include "userprog/plist.h"
#include "userprog/sys_file_calls.h"
#include "userprog/sys_proc_calls.h"

#define BOTTOM_VADDR_PTR ((void*)0x08048000)

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

bool is_valid_ptr(const void* ptr)
{
  if(!is_user_vaddr(ptr) || BOTTOM_VADDR_PTR > ptr)
    return false;
  
  return true;
}

bool verify_fix_length(void* start, int length)
{
  int* stop_addr = (int*)(start + length - 1);
  
  if(!is_user_vaddr((void*)stop_addr))
    return false;
  
  int* temp_addr = (int*)pg_round_down(start);
  
  for(; temp_addr <= stop_addr; temp_addr += PGSIZE)
    if(pagedir_get_page(thread_current()->pagedir, (void*)temp_addr) == NULL)
      return false;
  
  return true;
}

bool verify_variable_length(char* start)
{
  unsigned prev_page;
  
  if(pagedir_get_page(thread_current()->pagedir, start) == NULL)
    return false;
  else
    prev_page = pg_no(start);
  
  for (;;)
  {
    if (prev_page != pg_no(start))
    {
      if(pagedir_get_page(thread_current()->pagedir, start) == NULL)
        return false;
      
      prev_page = pg_no(start);
    }
    
    if(*start == '\0')
      return true;
    
    start++;
  }
}

void death()
{
  sys_exit(-1);
  thread_exit();
}

/* This array defined the number of arguments each syscall expects.
   For example, if you want to find out the number of arguments for
   the read system call you shall write:
   
   int sys_read_arg_count = argc[ SYS_READ ];
   
   All system calls have a name such as SYS_READ defined as an enum
   type, see `lib/syscall-nr.h'. Use them instead of numbers.
 */
const int argc[] = {
  /* basic calls */
  0, 1, 1, 1, 2, 1, 1, 1, 3, 3, 2, 1, 1, 
  /* not implemented */
  2, 1,    1, 1, 2, 1, 1,
  /* extended */
  0
};

static void
syscall_handler (struct intr_frame *f) 
{
  int32_t* esp = (int32_t*)f->esp;
  
  if(esp == NULL ||
     !is_user_vaddr((void*)esp) ||
     !verify_variable_length((void*)esp))
    death();
  
  switch (*esp)
  {
    case SYS_HALT:
    {
      power_off(); // power off
    }
    case SYS_EXIT:
    {
      if(!is_user_vaddr((void*)esp[1]))
	 death();
	 
      sys_exit(esp[1]);
      thread_exit();
    }
    case SYS_READ:
    {
      if(!verify_fix_length((void*)esp[2], esp[3]) ||
	 (char*)esp[2] == NULL)
      {
        death();
        break;
      }
      
      f->eax = sys_read(esp[1], (char*)esp[2], esp[3]);
      break;
    }
    case SYS_WRITE:
    {
      if(!verify_fix_length((char*)esp[2], esp[3]) || 
        (char*)esp[2] == NULL)
      {
        death();
        break;
      }
      
      f->eax = sys_write(esp[1], (char*)esp[2], esp[3]);
      break;
    }
    case SYS_OPEN:
    {
      if(!is_user_vaddr((void*)esp[1]) ||
	 !verify_variable_length((void*)esp[1]) ||
        (char*)esp[1] == NULL)
      {
        death();
        break;
      }
      
      f->eax = sys_open((char*)esp[1]);
      break;
    }
    case SYS_CREATE:
    {
      if(!verify_fix_length((void*)esp[1], esp[2]) || 
        (char*)esp[1] == NULL ||
        !is_user_vaddr((void*)esp[1]))
      {
        death();
        break;
      }
      
      f->eax = sys_create((char*)esp[1], esp[2]);
      break;
    }
    case SYS_REMOVE:
    {
      if((char*)esp[1] != NULL)
        f->eax = sys_remove((char*)esp[1]);
      else
        f->eax = 0;
      
      break;
    }
    case SYS_CLOSE:
    {
      sys_close(esp[1]); // void-funktion!
      break;
    }
    case SYS_SEEK:
    {
      sys_seek(esp[1], esp[2]);
      break;
    }
    case SYS_TELL:
    {
      f->eax = sys_tell(esp[1]);
      break;
    }
    case SYS_FILESIZE:
    {
      f->eax = sys_filesize(esp[1]);
      break;
    }
    /**
     * Nedan finns definierade sys_proc_calls.(h|c)
     **/
    case SYS_WAIT:
    {
      f->eax = sys_wait(esp[1]);
      break;
    }
    case SYS_PLIST:
    {
      sys_plist();
      break;
    }
    case SYS_SLEEP:
    {
      sys_sleep(esp[1]);
      break;
    }
    case SYS_EXEC:
    {
      if((void*)esp[1] != NULL && is_user_vaddr((void*)esp[1])
          && pagedir_get_page(thread_current()->pagedir, (void*)esp[1]) != NULL)
        f->eax = sys_exec((char*)esp[1]);
      else
        f->eax = -1;
      
      break;
    }
    default:
    {   
      thread_exit ();
    }
  }
}
