/* Sistemas Operativos
 * Guiao 4
 * 04/04/2016
 * Bruno Barbosa (a67646)
 * Objectivos: (1) familiarizar as chamadas ao sistema relativas
                   duplicacao de descritores e redirecionamento
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

#define MAX 32

/* Exercicio exemplo
 */
void example1(char *filename) {
  int fd;
  getchar();
  fd=open(filename,O_CREAT | O_WRONLY | O_TRUNC, 0666);
  getchar();
  dup2(fd,1);
  getchar();
  close(fd);
  getchar();
  printf("Hello My Friends!\n");
}

/* Exercicio 1
 * Redireciona o stdin para "entrada.txt"
 *               stdout para "saida.txt"
 *               stderr para "erros.txt"
 */
void redirect() {
  int fde,fds,fder,readed;
  int d0,d1,d2;
  char buffer[256];

  // open files
  fde = open("/etc/passwd", O_RDONLY);
  fds = open("saida.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  fder = open("erros.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  // save the original FDs
  d0 = dup(0);
  d1 = dup(1);
  d2 = dup(2);
  // duplicate the original FDs to the designated files
  dup2(fde,0);
  dup2(fds,1);
  dup2(fder,2);

  while((readed = read(0,buffer,256)) > 0) {
    write(1,buffer,readed);
    write(2,buffer,readed);
  }

  // restore original FDs
  dup2(d0,0);
  dup2(d1,1);
  dup2(d2,2);
  // close duplicated original FDs
  close(d0);
  close(d1);
  close(d2);
  // close files FDs
  close(fde);
  close(fds);
  close(fder);
}

/* Exercicio 2
 * Igual ao exercicio 1 mas um processo filho trata de
 * fazer a leitura e escrita
 * NOTA: Serve para demonstrar que o processo filho
 * preserva os FDs do pai
 */
void childRedirect() {
  int fde,fds,fder,readed,pid;
  int d0,d1,d2;
  char buffer[256];

  // open files
  fde = open("/etc/passwd", O_RDONLY);
  fds = open("fsaida.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  fder = open("ferros.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  // save the original FDs
  d0 = dup(0);
  d1 = dup(1);
  d2 = dup(2);
  // duplicate the original FDs to the designated files
  dup2(fde,0);
  dup2(fds,1);
  dup2(fder,2);

  pid = fork();
  if(pid == 0) {
    // child process
    while((readed = read(0,buffer,256)) > 0) {
      write(1,buffer,readed);
      write(2,buffer,readed);
    }
  }

  // close backup FDs
  dup2(d0,0);
  dup2(d1,1);
  dup2(d2,2);
  // close duplicated original FDs
  close(d0);
  close(d1);
  close(d2);
  // close files FDs
  close(fde);
  close(fds);
  close(fder);
}

/* Exercicio 3
 * Igual ao Exercicio 1 mas usa o WC
 */
void redirectWithWC() {
  int fde,fds,fder,readed;
  int d0,d1,d2;
  char buffer[256];

  // open files
  fde = open("/etc/passwd", O_RDONLY);
  fds = open("saida.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  fder = open("erros.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  // save the original FDs
  d0 = dup(0);
  d1 = dup(1);
  d2 = dup(2);
  // duplicate the original FDs to the designated files
  dup2(fde,0); // passwd
  //dup2(fds,1);
  //dup2(fder,2);

  //dup2(1,1); // print on terminal
  execlp("wc","wc",NULL);
  _exit(0);

  while((readed = read(0,buffer,256)) > 0) {
    write(fds,buffer,readed);
    write(fder,buffer,readed);
  }

  // restore original FDs
  dup2(d0,0);
  dup2(d1,1);
  dup2(d2,2);
  // close duplicated original FDs
  close(d0);
  close(d1);
  close(d2);
  // close files FDs
  close(fde);
  close(fds);
  close(fder);
}


// main
int main(int argc, char *argv[]) {
  int e;

  if(argc >= 2) {
      e = atoi(argv[1]);
      switch (e) {
        case 11 : {
          example1(argv[2]);
          break;
        }

        case 1 : {
          redirect();
          break;
        }

        case 2 : {
          childRedirect();
          break;
        }

        case 3 : {
          redirectWithWC();
          break;
        }
      }
  } else {
    printf("ERROR: Check the number of arguments\n");
    //example1("exemplo1.txt");
  }

  return 0;
}
