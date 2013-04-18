//
// Skeleton of bounded buffer for laboratory assignment 1 for Pintos
//
// Created by Andrzej Bednarski
//
// modified by Vlad Jahundovics for Pintos (translation from C++ to C)

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include "threads/synch.h"

struct bounded_buffer {
  int size;
  /**
  * Nedan från föreläsningsanteckningar
  **/
  //byte rpos, wpos = 0;
  //struct semaphore free = SIZE;
  //struct sempahore filled = 0;
  //struct lock buffer_mutex;
  // You may add other data fields required for the bounded buffer

};

void bb_init(struct bounded_buffer *, int);
int bb_read(struct bounded_buffer *);
void bb_write(struct bounded_buffer *, int);
void bb_destroy(struct bounded_buffer *);

#endif
