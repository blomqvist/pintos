#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "userprog/plist.h"

void process_init (void);
void process_print_list (void);
void process_exit (int status);
tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_cleanup (void);
void process_activate (void);

// Lab 17
void plist_print_row(p_key_t k, p_value_t v, int aux);
void process_exit_helper(p_key_t k, p_value_t v, int aux);
bool p_map_cleanup(p_key_t k, p_value_t v, int aux);

/* This is unacceptable solutions. */
#define INFINITE_WAIT() for ( ; ; ) thread_yield()
#define BUSY_WAIT(n)       \
    do {                   \
      int i = n;           \
      while ( i --> 0 )    \
        thread_yield();    \
    } while ( 0 )

#endif /* userprog/process.h */
