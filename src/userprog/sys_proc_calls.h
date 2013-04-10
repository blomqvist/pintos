#ifndef _sys_proc_calls_h
#define _sys_proc_calls_h

#include "userprog/plist.h"

void plist_print_row(p_key_t k, p_value_t v, int aux);
void sys_sleep(unsigned millis);
void sys_plist(void);
int sys_exec(char* command_line_arg);

#endif
