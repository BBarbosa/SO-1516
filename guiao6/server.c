/* Sistemas Operativos
 * Guiao 6
 * 02/05/2016
 * Bruno Barbosa (a67646)
 * Objectivos: (1) familiarizar as chamadas ao sistema relativas
                   a criacao de pipes com nome
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

#define LINE 256

// server code
int main(int argc, char *argv[]) {
  char b[LINE];
  int fd,fd2,r;

  if(argc < 3) exit(1);

  while(1) {
    fd = open(argv[1],O_RDONLY);
    fd2 = open(argv[2],O_WRONLY);

    if(fd == -1 || fd2 == -1) exit(1);

    if(fork()==0) {
      while((r=read(fd,b,256)) > 0) {
        write(1,b,r);
      }
      _exit(0);
    }

    while((r=read(0,b,256)) > 0) {
      write(fd2,b,r);
    }

    close(fd);
    close(fd2);
  }

  return 0;
}
