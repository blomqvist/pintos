//
//  sys_file_calls.c
//  
//
//  Created by Niklas Blomqvist on 2013-02-16.
//
//

#include <syscall-nr.h>
#include <string.h>
#include <stdio.h>

#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "threads/vaddr.h"

#include "filesys/filesys.h"
#include "filesys/file.h"
#include "filesys/off_t.h" // off_t (int32_t)

#include "devices/input.h" // input_getc()

#include "userprog/syscall.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "userprog/flist.h" // map
#include "userprog/sys_file_calls.h"

int sys_open(const char* file)
{
  struct file* temp = filesys_open(file);
  
  if(temp != NULL)
  {
    // Mata in pekaren till filen i mapen
    // filemap finns initierad i thread.h/c
    
    return map_insert(get_filemap(), temp); // returnerar värdet i mapen
  }
  return -1; // filen fanns inte
}

int sys_read(int fd, char* buffer, unsigned length)
{
  if (fd == STDOUT_FILENO)
    return -1; // Vi kan inte läsa från skärmen
  
  unsigned i = 0;
  char key;
  for (i = 0; i != length; i++)
  {
    if (fd == STDIN_FILENO) // Tangentbord
    {
      key = (char)input_getc();
      
      if (key == '\r') // enter
      {
        key = '\n'; // lägg till ny rad-tecken
        buffer[i] = key;
        break;
      }
      else
        buffer[i] = key;
      
      putbuf(&buffer[i], 1);
    }
    else
    {
      //Read from file
      struct file* read_from = map_find(get_filemap(), fd);
      
      if(read_from != NULL && length > 0)
        return file_read(read_from, buffer, length);
      else
        return -1;
    }
  }
  
  return length; // Så här många tecken läste jag
}

/**
 * Skriver till fd (skärm eller fil)
 * Returnerar antalet skrivna tecken eller -1 om antalet tecken som ska skrivas
 * är 0 eller om filen inte finns.
 */
int sys_write(int fd, const void* buffer, unsigned length)
{
  if (fd == STDIN_FILENO) // Vi kan inte skriva till tangentbordet
  {
    return -1;
  }
  
  if (fd == STDOUT_FILENO) // skärmen
  {
    putbuf(buffer, length);
    return length;
  }
  else
  {
    // Skriva till fil
    struct file* write_to = map_find(get_filemap(), fd);
    if(write_to != NULL)
    {
      // Skriver buffer till write_to. Returnerar antalet skrivna tecken
      // Kan returnera ett antal tecken < length om filen är för liten
      return file_write(write_to, buffer, length);
    }
    else
    {
      return -1; // Filen finns inte, eller så ville vi skriva 0 tecken.
    }
  }
  // Hit ska vi inte komma!
}

bool sys_create(const char* file, unsigned initial_size)
{
  if (filesys_create(file, initial_size) && initial_size >= 0)
  {
    map_insert(get_filemap(), filesys_open(file));
    return true;
  }
  return false;
}

bool sys_remove(const char* file)
{
  return filesys_remove(file);
}

void sys_close(int fd)
{
  struct file* close_file = map_find(get_filemap(), fd);
  
  if(close_file != NULL)
  {
    filesys_close(close_file);
    map_remove(get_filemap(), fd); // important to remove from file hEHE
  }
}

void sys_seek(int fd, unsigned position)
{
  struct file* file = map_find(get_filemap(), fd);
  
  if (file != NULL && position <= (unsigned)sys_filesize(fd))
    file_seek(file, position);
}

unsigned sys_tell (int fd)
{
  struct file* file = map_find(get_filemap(), fd);
  
  if (file != NULL)
    return file_tell(file);
  
  return -1;
}

int sys_filesize(int fd)
{
  struct file* file = map_find(get_filemap(), fd);
  
  if (file != NULL)
    return file_length(file);
  
  return -1;
}

struct map* get_filemap()
{
  return &thread_current()->filemap; //.node;
}
