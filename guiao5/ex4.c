#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

int main() {
  int fd[2];
  pipe(fd);

  // check ps on terminal
  if(fork() == 0) {
    // child
    close(fd[0]);
    dup2(fd[1],1);
    execlp("ls","ls","/etc",NULL);
  } else {
    // parent
    close(fd[1]);
    dup2(fd[0],0);
    execlp("wc","wc","-l",NULL);
  }
  return 0;
}
