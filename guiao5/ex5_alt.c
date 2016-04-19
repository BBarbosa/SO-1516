// pseudo code for exercise 5
void exercise5() {
  char** commands = ...;
  int** fds = ...
  for(int i = 0; i < size; i++) {
    int j = round_up(i/2);
    fds[j] = (int*)malloc(sizeof(int) * 2);
    int* fd = fds[j];
    pipe(fd);
    if(fork() == 0) {
      // child process
      dup2(fd[0],0);
      close(fd[0]);
      close(fd[1]);
      execlp(commands[i],...,NULL);
    } else {
      close(fd[0]);
      dup2(fd[1],1);
      close(fd[1]);
    }
  }
}
