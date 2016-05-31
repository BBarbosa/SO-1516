#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
  printf("PID: %d\nPPID: %d\n",getpid(),getppid());
  return 0;
}
