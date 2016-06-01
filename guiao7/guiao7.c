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

#define MAX 5

int var = 0;
int sec = 0;
int e = 0;
int proc = 0; // process ID

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
  sec++;
}

void show(int s) {
  printf("%d:%d:%d\n",sec/3600,sec/60,sec%60);
}

void exitP(int s) {
  e = 1;
}

void example3() {
  e = 0;
  signal(SIGALRM,hand13);
  signal(SIGINT,show);
  signal(SIGQUIT,exitP);
  printf("%d\n",getpid());
  while(!e) {
    alarm(1);
    pause();
  }
}

/* Exercicio 1
 * Programa que conta o tempo em segundos
 * Ctrl + C → imprime o tempo passado (SIGINT)
 * Ctrl + \ → imprime quantas vezes foi imprimido o tempo (SIGQUIT)
 *          → termina o programa
 */
void hand1(int s) {
  switch(s) {
    case SIGINT : {
      printf("%d:%d:%d\n",sec/3600,sec/60,sec%60);
      break;
    }

    case SIGQUIT : {
      e=1;
      break;
    }

    case SIGALRM : {
      sec++;
      break;
    }
  }
}

void exercise1() {
  signal(SIGINT,hand1);
  signal(SIGQUIT,hand1);
  signal(SIGALRM,hand1);

  e = 0; sec = 0;
  printf("%d\n",getpid());
  while(!e) {
    alarm(1);
    pause();
  }
}


/* Exercicio 2
 * Simulação Round Robin
 */
void hand2(int s) {
  switch(s) {
   case SIGCONT : {
     break;
   }

   case SIGSTOP : {
     break;
   }

   case SIGCHLD : {
     break;
   }
  }
}

int createProcess(char *p) {
  if(fork() == 0) {
    execlp(p,p,NULL);
  }
  return getpid();
}

/*
void exercise2(int nprocs, char *argv[]) {
  int i;

  for(i=0;i<nprocs;i++) {
    pids[i] = createProcess(argv[i+2]);
  }

  while(1) {
    for(i=0;i<nprocs;i++) {
      if(i==0) {
        kill(pids[(proc+i) % nprocs],SIGCONT);
      } else {
        kill(pids[(proc+i) % nprocs],SIGSTOP);
      }
    }
    sleep(2); puts("sleep");
    proc = (proc+1) % nprocs;
  }
}
*/

void exercise2(int nprocs, char *argv[]) {
  int pid;
  signal(SIGCONT,hand2);
  signal(SIGSTOP,hand2);
  signal(SIGCHLD,hand2);

  if(nprocs > 0) {
    if(fork() == 0) {
      exercise2(nprocs-1,argv+1);
      _exit(0);
    } else {
      pid = wait(0);
      execlp(*argv,*argv,NULL);
    }
  }
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
          exercise1();
          break;
        }

        case 2 : {
          exercise2(argc-2,argv+2);
          break;
        }
      }
  } else {
    printf("ERROR: Check the number of arguments\n");
    //example1("exemplo1.txt");
  }

  return 0;
}
