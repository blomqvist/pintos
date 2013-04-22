#include <debug.h>
#include <stdio.h>
#include <string.h>

#include "userprog/gdt.h"      /* SEL_* constants */
#include "userprog/process.h"
#include "userprog/load.h"
#include "userprog/pagedir.h"  /* pagedir_activate etc. */
#include "userprog/tss.h"      /* tss_update */
#include "filesys/file.h"
#include "threads/flags.h"     /* FLAG_* constants */
#include "threads/thread.h"
#include "threads/vaddr.h"     /* PHYS_BASE */
#include "threads/interrupt.h" /* if_ */
#include "lib/string.h"        /* strtok_r */

/* Headers not yet used that you may need for various reasons. */
#include "threads/synch.h"
#include "threads/malloc.h"
#include "lib/kernel/list.h"

#include "userprog/flist.h"
#include "userprog/plist.h"

#include "userprog/stack_helper.h"

/* HACK defines code you must remove and implement in a proper way */
#define HACK


/* This function is called at boot time (threads/init.c) to initialize
 * the process subsystem. */
void process_init(void)
{
  p_map_init(&p_map); // ADDED BY SEBKO Y NIKBLO
}

/* This function is currently never called. As thread_exit does not
 * have an exit status parameter, this could be used to handle that
 * instead. Note however that all cleanup after a process must be done
 * in process_cleanup, and that process_cleanup are already called
 * from thread_exit - do not call cleanup twice! */
void process_exit(int status)
{
  /**
   * Hämta ut en proc_table* från p_map
   * och sätt exit_status till status
   */
  
  p_map_for_each(&p_map, process_exit_helper, status);
}

/**
 * Sets alive status to false
 **/
void process_exit_helper(p_key_t k UNUSED, p_value_t v, int aux)
{
  if (v == NULL)
    return;
  
  if (v->parent_id == thread_current()->tid)
  {
    v->parent_alive = false;
  }
  
  if (v->proc_id == thread_current()->tid)
  {
    v->exit_status = aux;
  }
}

/* Print a list of all running processes. The list shall include all
 * relevant debug information in a clean, readable format. */
void process_print_list()
{
  p_map_for_each(&p_map, plist_print_row, 0);
}

void plist_print_row(p_key_t k UNUSED, p_value_t v, int aux UNUSED)
{
  if (v == NULL)
    return;
  
  printf("%d\t%d\t%s\t%d\t%s\t\t%s\n",
         v->proc_id,
         v->parent_id,
         v->alive ? "yes" : "no",
         v->exit_status,
         v->parent_alive ? "yes" : "no",
         v->proc_name);
}

/**
 * Parameters sent to start_process
 **/

struct parameters_to_start_process
{
  char* command_line;
  struct semaphore semaphore;
  bool success;
  tid_t parent_id;
};

static void
start_process(struct parameters_to_start_process* parameters) NO_RETURN;


/* Starts a new proccess by creating a new thread to run it. The
   process is loaded from the file specified in the COMMAND_LINE and
   started with the arguments on the COMMAND_LINE. The new thread may
   be scheduled (and may even exit) before process_execute() returns.
   Returns the new process's thread id, or TID_ERROR if the thread
   cannot be created. */
int
process_execute (const char *command_line) 
{
  char debug_name[64];
  int command_line_size = strlen(command_line) + 1;
  tid_t thread_id = -1;
  int  process_id = -1;

  /* LOCAL variable will cease existence when function return! */
  struct parameters_to_start_process arguments;
  
  /**
   * Initiate semaphore and aquire resource
   **/
  sema_init(&arguments.semaphore, 0);
  arguments.parent_id = thread_current()->tid;
  
  
  debug("%s#%d: process_execute(\"%s\") ENTERED\n",
        thread_current()->name,
        thread_current()->tid,
        command_line);
  
  
  /* COPY command line out of parent process memory */
  arguments.command_line = malloc(command_line_size);
  strlcpy(arguments.command_line, command_line, command_line_size);


  strlcpy_first_word (debug_name, command_line, 64);
  
  /* SCHEDULES function `start_process' to run (LATER) */
  thread_id = thread_create (debug_name, PRI_DEFAULT,
                             (thread_func*)start_process, &arguments);
  process_id = thread_id;
  
  if (process_id != -1)
  {
    sema_down(&arguments.semaphore);
  }
  
  /**
   * We must make sure that we are able to free command_line
   * before anyone else uses it again
   **/
  
  free(arguments.command_line);
  
  /**
   * Check if load i successful
   **/
  if (!arguments.success)
  {
    process_id = -1; // it was not
  }
  
  debug("%s#%d: process_execute(\"%s\") RETURNS %d\n",
        thread_current()->name,
        thread_current()->tid,
        command_line, process_id);

  /* MUST be -1 if `load' in `start_process' return false */
  return process_id;
}

/* A thread function that loads a user process and starts it
   running. */
static void
start_process (struct parameters_to_start_process* parameters)
{
  
  /* The last argument passed to thread_create is received here... */
  struct intr_frame if_;
  bool success;

  char file_name[64];
  strlcpy_first_word (file_name, parameters->command_line, 64);

  debug("%s#%d: start_process(\"%s\") ENTERED\n",
        thread_current()->name,
        thread_current()->tid,
        parameters->command_line);
  
  
  /* Initialize interrupt frame and load executable. */
  memset (&if_, 0, sizeof if_);
  if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
  if_.cs = SEL_UCSEG;
  if_.eflags = FLAG_IF | FLAG_MBS;

  success = load(file_name, &if_.eip, &if_.esp);
  
  
  debug("%s#%d: start_process(...): load returned %d\n",
        thread_current()->name,
        thread_current()->tid,
        success);
     
  if (success)
  {
    struct proc_table* p_table = malloc(sizeof(struct proc_table));
    
    p_table->proc_id = thread_current()->tid;
    p_table->parent_id = parameters->parent_id;
    p_table->alive = true;
    p_table->parent_alive = true;
    p_table->proc_name = malloc(16);
    strlcpy(p_table->proc_name, thread_current()->name, 16);
    sema_init(&p_table->semaphore, 0);
    
    p_map_insert(&p_map, p_table);
    
    if_.esp = setup_main_stack(parameters->command_line, (void*)PHYS_BASE);
    //dump_stack ( PHYS_BASE + 15, PHYS_BASE - if_.esp + 16 );
  }

  debug("%s#%d: start_process(\"%s\") DONE\n",
        thread_current()->name,
        thread_current()->tid,
        parameters->command_line);
  
  /* If load fail, quit. Load may fail for several reasons.
     Some simple examples:
     - File doeas not exist
     - File do not contain a valid program
     - Not enough memory
   */
  
  parameters->success = success;
  
  if (!success)
  {
    /**
     * Eftersom thread_exit() byter exekveringstråd måste vi
     * frigöra semaforen innan bytet (annars fastnar vi)
     */
    sema_up(&parameters->semaphore);
    thread_exit();
  }
  
  /**
   * Frigör semaforen innan return
   */
  sema_up(&parameters->semaphore);
  
  /* Start the user process by simulating a return from an interrupt,
     implemented by intr_exit (in threads/intr-stubs.S). Because
     intr_exit takes all of its arguments on the stack in the form of
     a `struct intr_frame', we just point the stack pointer (%esp) to
     our stack frame and jump to it. */
  asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (&if_) : "memory");
  NOT_REACHED ();
}

/* Wait for process `child_id' to die and then return its exit
   status. If it was terminated by the kernel (i.e. killed due to an
   exception), return -1. If `child_id' is invalid or if it was not a
   child of the calling process, or if process_wait() has already been
   successfully called for the given `child_id', return -1
   immediately, without waiting.

   This function will be implemented last, after a communication
   mechanism between parent and child is established. */
int
process_wait (int child_id) 
{
  int status = -1;
  struct thread *cur = thread_current();

  debug("%s#%d: process_wait(%d) ENTERED\n",
        cur->name, cur->tid, child_id);

  struct p_map* temp = &p_map;
  p_value_t t_child = NULL;
  
  // Iterera igenom p_map
  for (;;)
  {
    if (temp->value != NULL && temp->value->proc_id == child_id) // Vi har hittat child_thr
      t_child = temp->value;
    
    if (temp->next == NULL)
      break;
    temp = temp->next;
  }
  
  //TODO: Remove child when exit status is set
  //Sync t_child->alive check before using sema_down
  if (t_child != NULL)
  {
    if (t_child->alive)
      sema_down(&t_child->semaphore); // Låt om oss vänta på denna tråd.
    
    status = t_child->exit_status;
  }
  
  debug("%s#%d: process_wait(%d) RETURNS %d\n",
        cur->name, cur->tid, child_id, status);
  return status;
}

/* Free the current process's resources. This function is called
   automatically from thread_exit() to make sure cleanup of any
   process resources is always done. That is correct behaviour. But
   know that thread_exit() is called at many places inside the kernel,
   mostly in case of some unrecoverable error in a thread.

   In such case it may happen that some data is not yet available, or
   initialized. You must make sure that nay data needed IS available
   or initialized to something sane, or else that any such situation
   is detected.
*/
  
void
process_cleanup (void)
{
  struct thread  *cur = thread_current ();
  uint32_t       *pd  = cur->pagedir;
  int status = -1;
  
  debug("%s#%d: process_cleanup() ENTERED\n", cur->name, cur->tid);
  
  /**
   * Later tests DEPEND on this output to work correct. You will have
   * to find the actual exit status in your process list. It is
   * important to do this printf BEFORE you tell the parent process
   * that you exit.  (Since the parent may be the main() function,
   * that may sometimes poweroff as soon as process_wait() returns,
   * possibly before the prontf is completed.)
   **/
  
  struct p_map* temp = &p_map;
  while (temp != NULL)
  {
    p_value_t v = temp->value;
    if (v != NULL && v->proc_id == thread_current()->tid)
    {
      v->alive = false;
      status = v->exit_status;
      sema_up(&v->semaphore); // sema_down() i process_wait()
      break;
    }
    temp = temp->next;
  }
  
  map_remove_if(&cur->filemap, close_helper, 0);
  p_map_remove_if(&p_map, p_map_cleanup, thread_current()->tid);
  
  printf("%s: exit(%d)\n", thread_name(), status);
  
  /* Destroy the current process's page directory and switch back
     to the kernel-only page directory. */
  if (pd != NULL) 
  {
    /* Correct ordering here is crucial.  We must set
     cur->pagedir to NULL before switching page directories,
     so that a timer interrupt can't switch back to the
     process page directory.  We must activate the base page
     directory before destroying the process's page
     directory, or our active page directory will be one
     that's been freed (and cleared). */
    cur->pagedir = NULL;
    pagedir_activate (NULL);
    pagedir_destroy (pd);
    
    status = 1;
  }
  debug("%s#%d: process_cleanup() DONE with status %d\n",
        cur->name, cur->tid, status);
}

/**
 * p_map_cleanup added in lab 17
 * for further assistance call 911
 **/
bool
p_map_cleanup(p_key_t k UNUSED, p_value_t v, int aux)
{
  if (v == NULL)
    return false;
  
  if (v->parent_id == aux) // aux = thread_current()->tid
  {
    v->parent_alive = false;
  }
  
  if (!v->parent_alive && !v->alive)
  {
    // free malloc from start_process
    free(v->proc_name);
    free(v);
    return true;
  }
  
  return false;
}

/* Sets up the CPU for running user code in the current
   thread.
   This function is called on every context switch. */
void
process_activate (void)
{
  struct thread *t = thread_current ();

  /* Activate thread's page tables. */
  pagedir_activate (t->pagedir);

  /* Set thread's kernel stack for use in processing
     interrupts. */
  tss_update ();
}

