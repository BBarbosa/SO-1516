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

  int fd = open(argv[1],O_CREAT | O_WRONLY,0666);
  if(fd == -1) {
    exit(1);
  }

  int i;
  int it = 10*1024;
  int buffer[1024];

  memset(buffer,'a',1024);

  for(i=0;i<it;i++) {
    write(fd,buffer,1024);
  }

  close(fd);

  return 0;
}
