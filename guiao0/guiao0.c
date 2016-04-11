/* Sistemas Operativos
 * Guiao 0
 * 15/02/2016
 * Bruno Barbosa (a67646)
 * Objectivos: (1) uso de apontadores através de uma lista ligada
               (2) familiarização com ferramentas de debugging
 */

#include "stdio.h"
#include "stdlib.h"

#define MAX 1500

/* Estrutura de dados que contêm os lugares que ainda não
 * foram ocupados
 */
typedef struct intv {
  int first;
  int last;
  int block;
  struct intv *next;
} *Interval;

/* Função que inicia a estrutura de dados
 */
Interval init(Interval i) {
  i = (Interval) malloc(sizeof(struct intv));
  i->first = 0;
  i->last = MAX;
  i->block = 0;
  i->next = NULL;

  return i;
}

/* Função que imprime os dados da estrutura de dados
 */
void print(Interval i) {
  if(i!=NULL) {
    printf("Interval %d [%d , %d]\n",i->block,i->first,i->last);
    print(i->next);
  } else {
    puts("End");
  }
}

/* Função que verifica se a estrutura já está totalmente
 * ocupada, ou seja, quando o o primeiro lugar livre
 * coincide com o ultimo lugar livre, o último
 */
int isFull(Interval i) {
  int first,last;
  first = i->first;
  last = i->last;
  if(first == last) {
    return 1;
  } else {
    return 0;
  }
}

/* Função que verifica se um lugar está num determinado
 * intervalo já definido
 */
int inInterval(int ub, int lb, int n) {
  return ((n > lb) && (n < ub));
}

/* Função que reserva um lugar
 */
Interval reserva(Interval livres, int n, int *reservado) {
  if(n<0 || n>MAX) {
    printf("#ERROR: Invalid seat!");
    return livres;
  }

  if(livres == NULL) {
    printf("#ERROR: Null pointer!");
    return livres;
  }

  if(!isFull(livres)) {
    Interval before,copy;
    before = NULL;
    copy = livres;

    while(!inInterval(copy->last,copy->first,n) && copy != NULL) {
      copy = copy->next;
      before = copy;
    }

    if(copy != NULL) {
      Interval new = (Interval) malloc(sizeof(struct intv));
      new->first = copy->first;
      new->last = n;
      new->next = copy;

      copy->first = n+1;

      if(before == NULL) {
          livres = new;
          new->block = 0;
          copy->block += 1;
      } else {
        before->next = new;
        new->block = before->block + 1 ;
        copy->block += 1;
      }

      if(n<(*reservado)) {
        *reservado = n;
      }
    } else {
      printf("#ERROR: Seat already reserved!");
      return livres;
    }
  } else {
    printf("#ERROR: No more seats availabe!");
    *reservado = -1;
  }
  return livres;
}

struct intv* liberta(struct intv* livres, int lugar, int n) {
  return livres;
}

int main() {
  Interval i = NULL;
  int reservado = MAX;
  i = init(i);

  i = reserva(i,10,&reservado);
  i = reserva(i,256,&reservado);
  //i = reserva(i,5,&reservado);

  print(i);
  printf("Reservado %d\n",reservado);

  return 0;
}
