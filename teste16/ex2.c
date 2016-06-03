/* Sistemas Operativos
 * Teste - Exercício 2
 * 02/06/2016
 * Bruno Barbosa (a67646)
 * EXEMPLO: ./ctrl p1 p2 ... pn
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>

int sec=0;
int nprocs=0;

// handler function
void handler(int s) {
	switch(s) {
		case SIGALRM : {
			sec++;
			break;
		}

		case SIGCHLD : {
			printf("Time %d:%d:%d\n",sec/3600,sec/60,sec%60);
			nprocs--;
			break;
		}
	}
}

int main(int argc, char const *argv[]) {
	int i,pid;

	// define which signals it will handle
	signal(SIGALRM,handler);
	signal(SIGCHLD,handler);

	// set the number of processes
	nprocs = argc-1;

	// create child process
	for(i=0;i<nprocs;i++) {
		if(fork() == 0) {
			execlp(argv[i+1],argv[i+1],NULL);
			_exit(i+1);
		}
	}

	// 2 alternatives (comment the undesired one)
	// waits for children processes death - version 1 (maybe OK)
	/* 
	while(nprocs > 0) {
		alarm(1);
		pause();
	} */

	// alternative to the previous solution - version 2 (doesn't catch the last process)
	/**/
	for(i=0;i<nprocs;i++) {
		pid = wait(0); // waits until a child process dies
		printf("PID %d\n",pid); // prints its PID
	}
	

	puts("Finished!");

	return 0;
}

