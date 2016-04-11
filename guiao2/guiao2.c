/* Sistemas Operativos
 * Guiao 2
 * 29/02/2016
 * Bruno Barbosa (a67646)
 * Objectivos: (1) familiarizar as chamadas ao sistema para
                   criação e gestão de processos
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX 32

/* Exemplo que demonstra a incapacidade de controlar
 * concorrencia dos processos
 */
void beBapBop() {
  printf("Be\n");
  fork();
  printf("Bap\n");
  fork();
  printf("Bop\n");
}

/* Exemplo para perceber mais uma vez, a impossibilidade
 * controlar a concorrencia. Para alem disso, também serve
 * para demonstrar as funcionalidades das primitivas
 */
void example() {
  int status;
  int pid;

  pid = fork();
  if(pid == 0) {
    //filho
    printf("Filho %d de %d\n",pid,getppid());
    _exit(4);
  } else if (pid > 0) {
    //pai
    printf("Pai\n");
    wait(&status);
    printf("Filho %d morreu com o codigo %d\n",pid,WEXITSTATUS(status));
  } else {
    printf("ERROR: Cannot create a child process!");
  }

  printf("Apenas pai!\n");
}

/* Exercicio 1
 * Imprime o seu identificador e o identificador do seu pai
 * Confirmar com o comando ps
 */
void fatherID() {
  int pid;

  pid = fork();
  if(pid == 0) {
    //filho
    printf("Filho tem o processID %d\n",getpid());
    printf("Pai do Filho tem o processID %d\n",getppid());
  } else if (pid < 0) {
    //erro
    printf("ERROR: Cannot create a child process!");
  }
}

/* Exercício 2
 * Criar um processo filho. Ambos os processos devem imprimir os
 * seus processID assim como os dos seus pais
 *
 */
void fatherAndChildID() {
  int pid,status,fpid=84;

  pid = fork();
  if(pid == 0) {
    //filho
    printf("Filho - O meu processID %d\n",getpid());
    printf("Filho - O processID do meu Pai %d\n",getppid());
    _exit(0);
  } else if (pid > 0) {
    //pai
    printf("Pai - O meu processID %d\n",getpid());
    printf("Pai - O processID do meu Pai %d\n",getppid());
    fpid = wait(&status);
    printf("Pai - O meu filho tem processID %d\n",fpid);
  } else {
    //erro
    printf("ERROR: Cannot create a child process!");
  }
}

/* Exercicio 3
 * Criar 10 filhos sequencialmente. Cada uma deles deverá o seu
 * PID e o PID dos seus pais. Ao terminarem devem fazer exit com
 * o respetivo número de ordem
 */
void tenChildrenSerial() {
  int i,fpid,pid,status;

  for(i=0;i<10;i++) {
    pid = fork();
    if(pid==0) {
      //filho
      printf("Filho %d - O meu processID %d\n",i,getpid());
      printf("Filho %d - O processID do meu Pai %d\n",i,getppid());
      _exit(i+1);
    } else if(pid > 0) {
      //pai
      fpid = wait(&status);
      printf("Pai - Morreu o filho %d com o numero %d\n",fpid,WEXITSTATUS(status));
    } else {
      //erro
      printf("ERROR: Cannot create a child process!");
    }
  }
}


/* Exercicio 4
 * Criar 10 filhos em paralelo. Cada uma deles deverá o seu
 * PID e o PID dos seus pais. Ao terminarem devem fazer exit com
 * o respetivo número de ordem
 * NOTA: São precisos 2 ciclos for!
 *  1 - Onde o processo pai cria os processos filho
 *  2 - Onde o processo pai espera pela morte dos processos filho
 */
void tenChildrenParallel(int n) {
  int i,fpid,status,pid=1;

  for(i=0;i<n;i++) {
      pid = fork();
      if(pid > 0) {
        printf("Pai - Criei um filho com PID %d\n",pid);
      } else if (pid == 0) {
        printf("Filho %d - O meu pai tem PID %d\n",getpid(),getppid());
        _exit(1); //mata o filho de modo a não entrar no ciclo
      } else {
        printf("ERROR: Cannot create child process!\n");
      }
  }

  for(i=0;i<n;i++) {
    //if(pid > 0) {
      fpid = wait(&status);
      printf("Pai - Morreu o filho %d com codigo %d\n",fpid,WEXITSTATUS(&status));
    //}
  }
}

/* Exercicio 5
 * Criar descendencia de profundidade 10
 * NOTA: Experimentar com recursividade
 */
void filhosEmProfundidade(int p) {
  int pid;

  if(p>0) {
    pid = fork();
    if(pid==0) {
      printf("Filho %d de %d - Tenho profundidade %d\n",getpid(),getppid(),p);
      filhosEmProfundidade(p-1);
      _exit(0);
    } else {
      wait(0); //espera que o filho morra
    }
  }
}


/* Exercicio 6
 * Dada uma matriz com o numero de colunas muito superior
 * ao numero de linhas, fazer uma pesquisa por elemento
 * Funcao Auxiliar "procura" - cada linha da matriz será
 * dada a um processo
 */
int procura(int elem, int tam, int linha[]) {
  int encontrou=0,i=0;

  while(!encontrou && i<tam) {
    if(linha[i] == elem) {
      encontrou = 1;
    }
    i++;
  }
  return encontrou;
}

/* Versao 2 da funcao procura que verifica quantas vezes o elemento
 * a procurar aparece na linha da matriz
 */
int procuraV2(int elem, int tam, int linha[]) {
  int encontrou=0,i=0;

  while(i<tam) {
    if(linha[i] == elem) {
      encontrou++;
    }
    i++;
  }
  return encontrou;
}

/* Funcao que divide a procura de um elemento numa matriz em varios
 * processos, de modo a que cada processo procure numa linha
 */
void procuraNaMatriz(int elem, int n) {
  int matriz[n][MAX];
  int i,j,fpid,status,pid,res;

  // fill the matrix
  for(i=0;i<n;i++) {
    for(j=0;j<MAX;j++) {
      matriz[i][j] = random()%MAX;
      //printf("%3d ",matriz[i][j]);
    }
    //printf("\n");
  }
  //puts("-----------------------------------------------------------------");

  for(i=0;i<n;i++) {
      pid = fork();
      if(pid == 0) {
        //filho
        res = procura(elem,MAX,matriz[i]);
        if(res) {
          //printf("Filho - Eu tenho PID %d e encontrei o elemento na linha %d!\n",getpid(),i+1);
          _exit(i+1);
        } else {
          _exit(0);
        }
      } else if (pid > 0) {
        //pai
      } else {
        printf("ERROR: Cannot create child process!\n");
      }
  }

  for(i=0;i<n;i++) {
    fpid = wait(&status);
    if(WEXITSTATUS(status)>0) {
      printf("Pai - O filho %d encontrou o elemento na linha %d!\n",fpid,WEXITSTATUS(status));
    }
  }
}

/* Exercicio 7
 * Esqueleto do exercicio 6 mas com melhorias
 *  (1) Imprimir por ordem as linhas onde o elemento foi encontrado
 *  (2) (Opcional)
 */
void procuraNaMatrizV2(int elem, int n) {
  int matriz[n][MAX];
  int matches[n]; //vai conter quantos elementos foram encontrado em cada linha
  int i,j,fpid,status,pid,res;

  // fill the matrix
  for(i=0;i<n;i++) {
    matches[i] = 0;
    for(j=0;j<MAX;j++) {
      matriz[i][j] = random()%MAX;
      printf("%3d ",matriz[i][j]);
    }
    printf("\n");
  }
  //puts("-----------------------------------------------------------------");

  for(i=0;i<n;i++) {
      pid = fork();
      if(pid == 0) {
        // filho
        res = procuraV2(elem,MAX,matriz[i]);
        if(res) {
          _exit(res);
        } else {
          _exit(0);
        }
      } else if (pid > 0) {
        // pai
      } else {
        printf("ERROR: Cannot create child process!\n");
      }
  }

  // guarda no array matches todas as ocorrencias por linha de acordo com o PID
  pid = getpid();
  for(i=0;i<n;i++) {
    fpid = wait(&status);
    matches[fpid-pid-1] = WEXITSTATUS(status);
  }

  // imprime as linhas onde foram encontrados os elementos
  for(i=0;i<n;i++) {
    if(matches[i]) {
      printf("Na linha %d foram encontrados %d elementos!\n",i+1,matches[i]);
    }
  }

}

int main(int argc, char *argv[]) {

  if(argc >= 2) {
    int exercise = atoi(argv[1]);
    switch(exercise) {
      case 1 : {
        fatherID();
        break;
      }

      case 2 : {
        fatherAndChildID();
        break;
      }

      case 3 : {
        tenChildrenSerial();
        break;
      }

      case 4 : {
        tenChildrenParallel(5);
        break;
      }

      case 5 : {
        filhosEmProfundidade(5);
        break;
      }

      case 6 : {
        //(elemento,processos)
        procuraNaMatriz(3,5);
        break;
      }

      case 7 : {
        //(elemento,processos)
        procuraNaMatrizV2(3,10);
        break;
      }
    }
  } else {
    puts("ERROR: Check the number of arguments!");
  }

  return 0;
}
