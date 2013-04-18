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

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
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
  enum intr_level intr_lvl = intr_get_level();
  
  switch (*esp)
  {
    case SYS_HALT:
    {
      power_off(); // power off
    }
    case SYS_EXIT:
    {
      sys_exit(esp[1]);
      thread_exit();
    }
    case SYS_READ:
    {
      intr_set_level(INTR_ON);
      f->eax = sys_read(esp[1], (char*)esp[2], esp[3]);
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_WRITE:
    {
      intr_set_level(INTR_ON);
      f->eax = sys_write(esp[1], (char*)esp[2], esp[3]);
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_OPEN:
    {
      intr_set_level(INTR_ON);
      f->eax = sys_open((char*)esp[1]);
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_CREATE:
    {
      intr_set_level(INTR_ON);
      f->eax = sys_create((char*)esp[1], esp[2]);
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_REMOVE:
    {
      intr_set_level(INTR_ON);
      f->eax = sys_remove((char*)esp[1]);
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_CLOSE:
    {
      intr_set_level(INTR_ON);
      sys_close(esp[1]); // void-funktion!
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_SEEK:
    {
      intr_set_level(INTR_ON);
      sys_seek(esp[1], esp[2]);
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_TELL:
    {
      intr_set_level(INTR_ON);
      f->eax = sys_tell(esp[1]);
      intr_set_level(intr_lvl);
      break;
    }
    case SYS_FILESIZE:
    {
      intr_set_level(INTR_ON);
      f->eax = sys_filesize(esp[1]);
      intr_set_level(intr_lvl);
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
      f->eax = sys_exec((char*)esp[1]);
      break;
    }
    default:
    {   
      thread_exit ();
    }
  }
}
