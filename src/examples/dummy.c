/* klaar@ida

   A small dummy process that just uses up a process slot in the long
   runtime test. Just return it's first argument.

   Normally used by some parent process.
 */

#include <stdlib.h>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return 0;
  
  return atoi(argv[1]);
}
