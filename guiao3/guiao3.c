/* Sistemas Operativos
 * Guiao 3
 * 14/03/2016
 * Bruno Barbosa (a67646)
 * Objectivos: (1) familiarizar as chamadas ao sistema relativas
                   a execucao de programas
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX 32

/* Exercicio exemplo das funcionalidades das funcoes
 * Numero 11 na main
 */
void execute() {
  printf("Before\n");
  execlp("ps","ps","-l",NULL);
  //execl("/bin/ps","ps","-l",0);
  //system("ps -l");
  printf("After\n");
}

/* Exercicio exemplo de recursividade infinita
 * Numero 12 na main
 */
void infiniteRecursive(int argc, char *argv[]) {
  if(argc > 1) {
    execlp(argv[2],argv[2],NULL);
    execlp(argv[0],argv[0],"12",argv[2],NULL);
  }
}

/* Exercicio 1
 * Executar o comando ls -l
 */
void executeLS() {
  execlp("ls","ls","-l",NULL);
  _exit(0);
}

/* Exercicio 2
 * Executa o comando ls -l agora num processo filho
 */
void childExecuteLS() {
  int pid;

  pid = fork();
  if(pid == 0) {
    // child process
    printf("I'm a child process!\n");
    executeLS();
    _exit(0);
  } else if (pid > 0) {
    // parent process
    wait(0);
  } else {
    printf("ERROR: Can not create child process!\n");
  }
}

/* Exercicio 3
 * Imprime a lista de argumentos recebidos na linha de
 * comandos
 */
void printArguments(int argc, char *argv[]) {
  int i;
  for(i=0;i<argc;i++) {
    printf("argv[%d] = %s\n",i,argv[i]);
  }
}

/* Exercicio 4
 * Exatamente como o execicio 3 mas em vez de imprimir
 * executa com o a lista de argumentos
 * Experimentar fazendo variar argv[0]
 */
void executeListOfArguments(int argc, char *argv[]) {
  if(argc > 2) {
    execvp(argv[2],argv+2);
  }
}

/* Exercicio 5
 * Executa concorrentemente uma lista de executaveis
 * especificados como argumentos da linha de comandos
 */
void executeListOfPrograms(int argc, char *argv[]) {
  int i,pid;
  for(i=2;i<argc;i++) {
    pid = fork();
    if(pid == 0) {
      // child process
      execlp(argv[i],argv[i],NULL);
      _exit(0);
    }
  }

  for(i=2;i<argc;i++) {
    wait(0);
  }
  printf("INFO: All programs ended!\n");
}

/* Exercicio 6
 * Minha versao da funcao system
 */
void mySystem(int argc, char *argv[]) {
  int r;
  if (argc > 2) {
    if(fork() == 0) {
      //execl("/bin/sh","/bin/sh","-c",argv+2,NULL);
      execvp("grep",argv+2);
    } else {
      
    }

  }
}

/* Exercicio 7
 * Interpretador de comandos estilo bash
 * le, analisa o ultimo caracter, e executa
 */
void myBash() {

}

// main
int main(int argc, char *argv[]) {
  int e;

  if(argc >= 2) {
      e = atoi(argv[1]);
      switch (e) {
        case 11 : {
          execute();
          break;
        }

        case 12 : {
          infiniteRecursive(argc,argv);
          break;
        }

        case 1 : {
          executeLS();
          break;
        }

        case 2 : {
          childExecuteLS();
          break;
        }

        case 3 : {
          printArguments(argc,argv);
          break;
        }

        case 4 : {
          executeListOfArguments(argc,argv);
          break;
        }

        case 5 : {
          executeListOfPrograms(argc,argv);
          break;
        }

        case 6 : {
          mySystem(argc,argv);
          break;
        }

        case 7 : {
          myBash();
        }
      }
  }

  return 0;
}
