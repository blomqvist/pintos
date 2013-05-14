#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
bool is_valid_ptr(const void* ptr);
bool verify_fix_length(void* start, int length);
bool verify_variable_length(char* start);
void death(void);

#endif /* userprog/syscall.h */
