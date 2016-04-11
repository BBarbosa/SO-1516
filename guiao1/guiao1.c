/* Sistemas Operativos
 * Guiao 1
 * 15/02/2016
 * Bruno Barbosa (a67646)
 * Objectivos: (1) familiarizar as chamadas ao sistema para
                   operações sobre ficheiros
 */

/* descitores:
    0 - STDIN
    1 - STDOUT
    3 - STDERR
*/

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE 1024

/* Exercício 1 */
void mycat() {
  char buffer[128];
  int readed=1;

  /* alternativa
  while(readed > 0) {
      readed = read(0,buffer,1);
      writed = write(1,buffer,readed);
  }
  */

  while((readed = read(0,buffer,1)) > 0) {
    write(1,buffer,readed);
  }
}

/* Exercício 2 */
void tenMegaBytes(char *filename) {
  /* 10 * 1 byte * 1024 (KB) * 1024 MB */
  int tenMB = 10 * 1024 * 1024;

  int fd = open(filename,O_CREAT | O_WRONLY,0666);
  int i=0;

  if(fd != -1) {
    while(i<tenMB) {
      write(fd,"a",1);
      i++;
    }
  } else {
    puts("ERROR: Cannot open the designated file!");
  }

  close(fd);
}

/* Exercício 3 */
void mycatV2() {
  char buffer[1024];
  int readed;

  while((readed = read(0,buffer,1024)) > 0) {
    write(1,buffer,readed);
  }
}

/* Exercício 4
 * Comparar o tempo com o Exercício 2 onde eram lido
 * caracter a caracter
 * Agora a leitura/escrita é feita por blocos de tamanho
 * N
 */
void tenMegaBytesV2(char *filename) {
  /* 1 Kbyte * 1024 MB */
  int tenMB = 10 * 1024;
  int fd = open(filename,O_CREAT | O_WRONLY,0666);
  int i=0;
  /* Buffer com 1 KB */
  char buffer[1024];
  /* Preenche o buffer de 1KB com o caracter 'a' */
  memset(buffer,'a',1024);
  /* Verifica se consegue abrir o ficheiro */
  if(fd != -1) {
    while(i<tenMB) {
      write(fd,buffer,1024);
      i++;
    }
  } else {
    puts("ERROR: Cannot open the designated file!");
  }
  close(fd);
}

/* Exercicio 5
 * Funcao que lê uma linha de cada vez
 */
int readln(int fd, void *buffer, int bytes) {
  int readed,i=0;
  char *aux = (char *) buffer;

  while(i<bytes && (readed = read(fd,&aux[i],1) > 0) && (aux[i] != '\n')) {
    i++;
  }

  return (readed == -1 ? -1 : i);
}

/* Exercicio 6
 * A minha implementacao do newline
 * Usando 2 buffers: (1) para o numero da linha
 *                   (2) para o texto lido
 * NOTA: Problema ao escrever a ultima linha!
 * Escreve um número depois da ultima leitura valida.
 */
void myNL(int fd, void  *buffer, int bytes) {
  int readed,i=1,aux;
  char toWrite[16];

  while((readed = readln(fd,buffer,bytes)) > 0) {
    aux = sprintf(toWrite,"     %d ",i);
    write(1,toWrite,aux);
    write(1,buffer,readed);
    write(1,"\n",1);
    i++;
  }
}

/* Exercicio 6
 * Implementacao alternativa para o newline
 * Usando apenas um buffer quer para linha, quer para o texto
 */
void myNL_V2(int fd, void *buffer, int bytes) {
  int readed,i=1;

  while((readed = readln(fd,buffer+12,bytes)) > 0) {
    sprintf(buffer,"     %d ",i);
    write(1,buffer,readed+12);
    write(1,"\n",1);
    i++;
  }
}

/* Estrutura que vai conter varias linhas de leitura

typedef struct Lines {
  char line[LINE_SIZE];
  struct lines *next;
} *lines;

lines initStructLines(lines l) {
  l = (lines) malloc(sizeof(struct Lines));
  l->next = NULL;

  return l;
}

lines insertLine(lines l, char *line) {
  if(l==NULL) {
    l = initStructLines(l);
  } else {
    lines aux = l;
    while(aux->next != NULL) {
      aux = aux->next;
    }
    lines new = (lines) malloc(sizeof(struct Lines));
    strcpy(new->line,line);
    new->next = NULL;
    aux->next = new;
  }

  return l;
}

*/

/* Estrutura que contem as linhas de leitura de um
 * determinado descritor de ficheiro

typedef struct buffer {
    struct Lines l;
    int fd;
} *sbuffer
 */


/* Exercicio 7
void readlnV2(int fd, void *buffer, int bytes) {

}

 */


/* Exercício 8
 */

/***********************************************************
                      Exercicio Adicionais
 ***********************************************************/

/* Exercício 9
 * Reimplementar o programa mycat() para múltiplos ficheiros
 */
void mycatV3(int argc, char *argv[]) {
  char buffer[1024];
  int readed,i,fd;

  for(i=2; i<argc; i++) {
    fd = open(argv[i],O_RDONLY);

    while((readed = read(fd,buffer,1024)) > 0) {
      write(1,buffer,readed);
    }

    close(fd);
  }
}

/* Exercício 10
 */

/* Exercício 11
 * A minha implementação do comando head
 */
void myhead(int nlines) {

}


/* main
 * sintaxe do comando para correr
 * ./run <numero_do_exercicio> <argumentos_adicionais>
*/
int main(int argc, char *argv[]) {
  char buffer[256];

  if(argc >= 2) {
    int exercise = atoi(argv[1]);
    switch(exercise) {
      case 1 : {
        mycat();
        break;
      }

      case 2 : {
        tenMegaBytes("tenMB.dat");
        break;
      }

      case 3 : {
        mycatV2();
        break;
      }

      case 4 : {
        tenMegaBytesV2("tenMB-v2.dat");
        break;
      }

      case 5 : {
        readln(0,buffer,64);
        break;
      }

      case 61 : {
        myNL(0,buffer,64);
        break;
      }

      case 62 : {
        myNL_V2(0,buffer,128);
        break;
      }

      case 9 : {
        mycatV3(argc,argv);
        break;
      }

      case 10 : {
        break;
      }

      case 11 : {
        if(argc >= 3) {
          int line = atoi(argv[2]);
        } else {
          puts("ERROR: Exercise 11 needs 3 arguments!");
        }
        break;
      }
    }
  } else {
    puts("ERROR: Check the number of arguments!");
  }

  return 0;
}
