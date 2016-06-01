#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

int main() {
  char buffer[64];
  int fd[2],r;
  pipe(fd);

  // check ps on terminal
  if(fork() == 0) {
    // read
    close(fd[1]);
    while((r = read(fd[0],buffer,64)) > 0) {
      write(1,buffer,r);
    }
    close(fd[0]);
  } else {
    // write
    close(fd[0]);
    while((r = read(0,buffer,64)) > 0) {
      write(fd[1],buffer,r);
    }
  }
  return 0;
}
