#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int nptrs;
#define SIZE 100
  void *buffer[SIZE];
  nptrs = backtrace (buffer, SIZE);
  
  char **strings = backtrace_symbols(buffer, nptrs);
  if (strings == NULL)
    return 1;
  int i = 0;
  while (i < nptrs)
    printf("%s\n", strings[i++]);

  //free(strings);
  return 0;
}
