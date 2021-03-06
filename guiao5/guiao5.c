/* Sistemas Operativos
 * Guiao 5
 * 11/04/2016 
 * Bruno Barbosa (a67646)
 * Objectivos: (1) familiarizar as chamadas ao sistema relativas
                   a criacao de pipes anonimos
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

#define MAX 32

/* Exercicio Exemplo 1
 * Assumimos que sabemos quantos bytes vao ser lidos
 * Linha de comandos continua a espera de texto
 * O processo filho consegue enviar coisas para o pai!!!
 */
void example1() {
  int fd[2];
  char buf[32];

  pipe(fd);

  if(fork() == 0) {
    // child process
    //write(fd[1],"abc",4);
    write(fd[1],"a",1); write(fd[1],"bc",2);
  } else {
    read(fd[0],buf,3);
    write(1,buf,3);
    putchar('\n');
  }
}

/* Exercicio Exemplo 2
 * Quando nao sabemos quandos quantos bytes vamos ler
 * Fica continuamente a espera de leitura
 */
void example2() {
  int fd[2];
  char ch;

  pipe(fd);

  if(fork() == 0) {
    // child process
    write(fd[1],"abc",3);
  } else {
    /**/
    while((read(fd[0],&ch,1))>0 && ch != 0) {
      write(1,&ch,1);
    }
    /* variar
    while((read(fd[0],&ch,1))>0) {
      write(1,&ch,1);
    }
    */
  }
}

/* Exercicio Exemplo 3
 * Com o fecho dos descritores do pipe devidamente conseguimos
 * resolver os problemas do exercicio exemplo 2
 */
void example3() {
  int fd[2];
  char ch;

  pipe(fd);

  if(fork() == 0) {
    // child process
    close(fd[0]); // fecha descritor de leitura do pipe pois vai apenas escrever
    write(fd[1],"abc",3);
  } else {
    close(fd[1]); // fehca descritor de escrita do pipe porque so vai ler
    while((read(fd[0],&ch,1))>0 && ch != 0) {
      write(1,&ch,1);
    }
    /*
    while((read(fd[0],&ch,1))>0) {
      write(1,&ch,1);
    }
    */
  }
}

/* Exercicio Exemplo 4
 * Faz o mesmo que:
 * $ ps | sort
 */
void ps_sort() {
  int fd[2];

  pipe(fd);

  if(fork() == 0) {
    // child process
    dup2(fd[1],1); // redireciona o STDOUT para o PIPE[1]
    close(fd[0]);  // fecha descritor para nao ficar continuamente a ler
    close(fd[1]);  // fecha descritor para nao ficar continuamente a escrever
    execlp("ps","ps",NULL);
  } else {
    dup2(fd[0],0); // redireciona o STDIN para o PIPE[0]
    close(fd[0]);  // fecha descritor para nao ficar continuamente a ler
    close(fd[1]);  // fecha descritor para nao ficar continuamente a escrever
    execlp("sort","sort",NULL);
  }
}

/* Exercicio 1
 * Enviar mensagens entre processos pai e filho
 */
void exercise1() {
  int fd[2],r;
  char buf[32];

  pipe(fd);

  if(fork() == 0) {
    // child process

    /* pai para filho */
    r = read(fd[0],buf,5);
    write(1,buf,r); putchar('\n');

    /* filho para pai
    sleep(5);
    write(fd[1],"Hello",5); */

  } else {
    /* pai para filho */
    sleep(5);
    write(fd[1],"Hello",5);

    /* filho  para o pai
    r = read(fd[0],buf,5);
    write(1,buf,r); putchar('\n'); */
  }
}


/* Exercicio 2
 * Fazer leitura ate encontrar EOF
 * Sentido filho para o pai (caso contrario, o processo
 * filho fica a correr em background)
 */
void exercise2() {
 int fd[2];
 char c1,c2;

 pipe(fd);

 if(fork() == 0) {
   // child process
   close(fd[0]);
   while(read(0,&c2,1) > 0) {
     write(fd[1],&c2,1);
   }
   close(fd[1]);
 } else {
   close(fd[1]);
   while(read(fd[0],&c1,1) > 0) {
      write(1,&c1,1);
   }
   close(fd[0]);
 }
}

/* Exercicio 3
 * Processo pai envia linhas de texto
 * Processo filho executa wc
 */
void exercise3() {
  int fd[2];
  char c1;

  pipe(fd);

  if(fork() == 0) {
    // child process
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);
    execlp("wc","wc",NULL);
  } else {
    close(fd[0]);
    while(read(0,&c1,1) > 0) {
      write(fd[1],&c1,1);
    }
    close(fd[1]);
  }
}

/* Exercicio 4
 * Emular o funcionamento de
 * $ ls /etc | wc -l
 */
void exercise4() {
  int fd[2];

  pipe(fd);

  if(fork() == 0) {
    // child process
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);
    execlp("wc","wc","-l",NULL);
  } else {
    close(fd[0]);
    dup2(fd[1],1);
    execlp("ls","ls","/etc",NULL);
    close(fd[1]);
  }
}

/* Exercicio 5
 * $ grep -v ^# /etc/passwd | cut -f7 -d: | uniq | wc -l
 * Sentido: filho para o pai
 */
void exercise5() {
  int fd1[2],fd2[2],fd3[2];

  pipe(fd1);

  if(fork() == 0) {
    // child process 1
    pipe(fd2);
    if(fork() == 0) {
      // child process 2
      pipe(fd3);
      if(fork() == 0) {
        // child process 3
        close(fd3[0]);
        dup2(fd3[1],1); // redireciona o STOUT para o pipe
        execlp("grep","grep","-v","^#","/etc/passwd",NULL);
        close(fd3[0]);
        close(fd3[1]);
        _exit(1);
      } else {
        wait(0); // espera pela morte do filho para seguir a execucao
        dup2(fd3[0],0); // redireciona o STDIN do pipe
        dup2(fd2[1],1); // redireciona o STOUT para o pipe
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);
        execlp("cut","cut","-f7","-d:",NULL);
        _exit(1);
      }
    } else {
      dup2(fd2[0],0);
      dup2(fd1[1],1);
      close(fd1[0]);
      close(fd1[1]);
      close(fd2[0]);
      close(fd2[1]);
      execlp("uniq","uniq",NULL);
      wait(0); // espera pela morte do filho para seguir a execucao
      _exit(1);
    }
  } else {
    dup2(fd1[0],0);
    close(fd1[1]);
    close(fd1[0]);
    execlp("wc","wc","-l",NULL);
    wait(0);
  }
}

/* Exercicio 5 (Alternativa)
 * Algoritmo que garante a execucao de qualquer pipeline
 * Executa dinamicamente.
 */
void exercicio5_alt(char *argv[], int argc) {
  int i,p,fd[2],v;

  // counts how many pipes exist (using char ',')
  for(i=0,p=1;i<argc;i++) {
    v=0;
    if(strcmp(argv[i],",") == 0) {
      v=1;
      p++;
      argv[p] = strcpy(argv[p],argv[i]);
    }

    if(i != 0 && v != 1){
      argv[p] = strcat(argv[p],argv[i]);
    }
  }

  for(i=0;i<p;i++) {
    printf("argv[%d] = %s\n",i,argv[i]);
  }
  /*
  for(i=0;i<p;i++) {
    if(fork() == 0) {
      dup2(fd[0],0);
      close(fd[0]);
      close(fd[1]);

    }
  }
  */

  printf("There are %d pipes\n",p);
}

/* Exercicio 6
 */
void exercise6(char *argv[], int argc) {

}

/* Exercicio 7
 */
void exercise7() {

}

// main
int main(int argc, char *argv[]) {
  int e;

  if(argc >= 2) {
      e = atoi(argv[1]);
      switch (e) {
        case 11 : {
          example1();
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

        case 14 : {
          ps_sort();
          break;
        }

        case 1 : {
          exercise1();
          break;
        }

        case 2 : {
          exercise2();
          break;
        }

        case 3 : {
          exercise3();
          break;
        }

        case 4 : {
          exercise4();
          break;
        }

        case 5 : {
          exercise5();
          break;
        }

        case 51 : {
          exercicio5_alt(argv+2,argc-2);
          break;
        }

        case 6 : {
          exercise6(argv+2,argc-2);
          break;
        }

        case 7 : {
          exercise7();
          break;
        }
      }
  } else {
    printf("ERROR: Check the number of arguments\n");
    //example1("exemplo1.txt");
  }

  return 0;
}
