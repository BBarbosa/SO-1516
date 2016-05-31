#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if(argc < 2) {
    exit(1);
  }

  int r;
  int n = atoi(argv[1]);
  char buffer[n];

  while((r = read(0,buffer,n)) > 0) {
    write(1,buffer,r);
  }

  /*
  r = read(0,buffer,n);
  write(1,buffer,r);
  */

  return 0;
}
