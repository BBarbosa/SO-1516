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

  if(fork() == 0) {
    dup2(fd[0],0);
    r = read(0,buffer,64);
    write(1,buffer,r);
  } else {
    sleep(2);
    //write(fd[1],"ola\n",4);
    dup2(fd[1],1);
    write(1,"andre\n",6);
  }

  return 0;
}
