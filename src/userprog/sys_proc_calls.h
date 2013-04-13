#ifndef _sys_proc_calls_h
#define _sys_proc_calls_h

#include "userprog/plist.h"

void sys_sleep(unsigned millis);
void sys_plist(void);
int  sys_exec(char* command_line_arg);
void sys_exit(int status);
int  sys_wait(pid_t id);
#endif
