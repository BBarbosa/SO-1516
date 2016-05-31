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


// server code
int main(int argc, char *argv[]) {
  char b;
  int fd;

  if(argc < 2) exit(1);

  while(1) {
    fd = open(argv[1],O_RDONLY);
    // ends with Ctrl + D
    while(read(fd,&b,1) > 0) {
      write(1,&b,1);
    }
    close(fd);
  }

  return 0;
}
