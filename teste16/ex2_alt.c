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
	switch (s) {
		case SIGALRM : {
			sec++;
			break;
		}
		
		case SIGKILL : {
			printf("%d:%d:%d\n",sec/3600,sec/60,sec%60);
			break;
		}

		case SIGUSR1 : {
			nprocs--;
		}
	}
	
}

int main(int argc, char *argv[]) {
	int i,pid1,pid2,status;

	// set the number of processes
	nprocs = argc-1;

	signal(SIGALRM,handler);
	signal(SIGKILL,handler);
	signal(SIGUSR1,handler);

	// create child process
	for(i=0;i<nprocs;i++) {
		if(fork()==0) {
			// this process executes the program
			pid1 = fork();
			if(pid1 == 0) {
				execlp(argv[i+1],argv[i+1],NULL);
				_exit(0);
			}

			// this process prints OK
			pid2 = fork();
			if(pid2 == 0) {
				signal(SIGKILL,handler);
				signal(SIGALRM,handler);
				while(1) {
					alarm(3);
					pause(); 
					printf("%d - OK\n",pid1);
				}
				_exit(0);
			}

			// waits until PID1 dies
			waitpid(pid1,&status,WUNTRACED);
			// when PID1 is gone, it kills PID2
			kill(pid2,SIGKILL); 
			// send signal to parent to decrement nprocs variable
			kill(getppid(),SIGUSR1);
			_exit(0);
		}
	}
	
	while(nprocs > 0){
		alarm(1);
		pause();
	}

	puts("All Finished!");

	return 0;
}

