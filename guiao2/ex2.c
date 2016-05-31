#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
  int pid = fork();

  if(pid == 0) {
    sleep(1);
    printf("Child - PID: %d\n",getpid());
    printf("Child - PPID: %d\n",getppid());
  } else if (pid > 0) {
    printf("Parent - PID: %d\n",getpid());
    printf("Parent - PPID: %d\n",getppid());
  } else {
    puts("Error: Couldn't create child process!");
  }

  return 0;
}
