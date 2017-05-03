#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>

static const int MAXSEM = 10;
int semphr;
int shm;
pid_t ppid;
int *clients;

union semun{
	int val;
};

void finish(int signo){
	for(int i=0; i<(sizeof(clients)/sizeof(int)); i++){
		if(clients[i]!=0) kill(clients[i], SIGINT);
	}
	semctl(semphr, 0, IPC_RMID, 0);
	shmdt(shmat(shm, NULL, 0));
	shmctl(shm, IPC_RMID, NULL);
	exit(signo);
}

int main(int argc, char *argv[]){
	if(argc<3) return 1;
	ppid = getpid();
	int clientcnt = atoi(argv[1]);
	int cutcnt = atoi(argv[2]);
	clients = calloc(clientcnt, sizeof(int));
	int clientrnr=0;
	semphr = semget(ftok(getenv("HOME"), MAXSEM), MAXSEM, 0777);
	shm = shmget(ftok(getenv("HOME"), MAXSEM), 128, 0777);
	union semun attr;
	attr.val=2;
	for(int i=0; i<MAXSEM; i++)semctl(semphr, i, SETVAL, attr);
	pid_t pid;
	for(int i=0; i<clientcnt; i++){
		if((pid=fork())==0) break;
		else if(pid<0){
			printf("Forking problem\n");
			finish(1);
		}
		clients[clientrnr++]=pid;
		signal(SIGINT, finish);
	}
	if(pid==0){
		int place;
		while(true){
			for(int i=0; i<MAXSEM; i++){
				if(semctl(semphr, i, SETVAL, attr)==2) place=i;
			}
			semop();
			
			
			semop();
		}
	}else{
		
	}
	
	return 0;
}
