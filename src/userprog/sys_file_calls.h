//
//  sys_file_calls.h
//  
//
//  Created by Niklas Blomqvist on 2013-02-16.
//
//

#ifndef _sys_file_calls_h
#define _sys_file_calls_h

int sys_open(const char* file);
int sys_read (int fd, char* buffer, unsigned length);
int sys_write(int fd, const void *buffer, unsigned length);
bool sys_create(const char* file, unsigned initial_size);
bool sys_remove(const char* file);
void sys_close(int fd);

void sys_seek(int fd, unsigned position);
unsigned sys_tell (int fd);
int sys_filesize(int fd);

struct map* get_filemap();

#endif
