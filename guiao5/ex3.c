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
    dup2(fd[0],0);
    // r = read(fd[0],buffer,64);
    execlp("wc","wc",NULL);
    _exit(0);
  } else {
    // write
    close(fd[0]);
    r = read(0,buffer,64);
    write(fd[1],buffer,r);
    close(fd[1]);
  }
  return 0;
}
