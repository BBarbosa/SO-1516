/* Sistemas Operativos
 * Guiao 7
 * 02/05/2016
 * Bruno Barbosa (a67646)
 * Objectivos: (1) familiarizar as chamadas ao sistema relativas
                   a criacao/manipulacao de sinais
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>


int var = 0;
int seg = 0;

void hand(int s) {

}

/* Exercicio exemplo 1 */
void example1() {
  printf("%d\n",getpid());
  signal(SIGUSR1,hand);
  pause();
  while(1) {
    printf("bip\n");
    sleep(5);
  }
}

/* Exercicio exemplo 2 */
void hand121(int s) {
  var++;             // if SIGUSR1 var++
  if(s == SIGUSR2) { // if SIGURS2 var += 2
    var++;
  }
}

void hand122(int s) {
  var--;
}

void example2() {
  signal(SIGUSR1,hand121); // + 1
  signal(SIGUSR2,hand121); // + 2
  signal(SIGINT,hand122); // Ctrl+C

  printf("My pid is %d\n",getpid());

  while(1) {
    pause();
    printf("%d\n",var);
  }
}

/* Exercicio exemplo 3 */
void hand13(int s) {
  seg++;
}

void show(int s) {
  printf("%d:%d:%d\n",seg/3600,seg/60,seg%60);
}

void example3() {
  signal(SIGALARM,hand3);
  signal(SIGINT,show);

  while(1) {
    alarm(1);
    pause();
  }
}

/* Exercicio 1
 * Programa que conta o tempo em segundos
 * Ctrl + C → imprime o tempo passado
 * Ctrl + \ → imprime quantas vezes foi imprimido o tempo
 *          → termina o programa
 */
void hand1(int s ) {

}
void exercise1() {

}

// main
int main(int argc, char *argv[]) {
  int e;

  if(argc >= 2) {
      e = atoi(argv[1]);
      switch (e) {
        case 11 : {
          example1(1);
          break;
        }

        case 12 : {
          example2();
          break;
        }

        case 13 : {
          example3();
          break;
        }

        case 1 : {

        }
      }
  } else {
    printf("ERROR: Check the number of arguments\n");
    //example1("exemplo1.txt");
  }

  return 0;
}
