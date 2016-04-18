/* Sistemas Operativos
 * Guiao 6
 * 18/04/2016
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

/* Exercicio 1
 * Assumimos que sabemos quantos bytes vao ser lidos
 * Linha de comandos continua a espera de texto
 * O processo filho consegue enviar coisas para o pai!!!
 */


// main
int main(int argc, char *argv[]) {
  int e;

  if(argc >= 2) {
      e = atoi(argv[1]);
      switch (e) {

      }
  } else {
    printf("ERROR: Check the number of arguments\n");
    //example1("exemplo1.txt");
  }

  return 0;
}
