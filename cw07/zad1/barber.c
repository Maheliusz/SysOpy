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
#include<time.h>
#include"queue.h"

int semphr;
int shm;
pid_t ppid;
int *queue;

union semun{
	int val;
};

void finish(int signo){
	printf("Golibroda konczy prace\n");
	semctl(semphr, 0, IPC_RMID, 0);
	shmdt(queue);
	shmctl(shm, IPC_RMID, NULL);
	exit(signo);
}

int main(int argc, char *argv[]){
	ppid = getpid();
	semphr = semget(ftok(getenv("HOME"), MAXSEM), MAXSEM+1, 0777 | IPC_CREAT);
	shm = shmget(ftok(getenv("HOME"), MAXSEM), 10*sizeof(int), 0777 | IPC_CREAT);
	queue = (int *)shmat(shm, NULL, 0);
	union semun attr;
	attr.val=1;
	for(int i=1; i<MAXSEM+1; i++)semctl(semphr, i, SETVAL, attr);
	attr.val=0;
	semctl(semphr, 0, SETVAL, attr);
	signal(SIGINT, finish);
	struct timespec tmspec;
	struct sembuf buf;
	int rnr=0;
	int clientid;
	signal(SIGINT, finish);
	while(1){
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zasypia\n", (int)tmspec.tv_sec);
		buf.sem_num=0;
		buf.sem_op=-1;
		semop(semphr, &buf, 1);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zostaje obudzony\n", (int)tmspec.tv_sec);
		while((clientid=queue[rnr])==-1){
			rnr=(rnr+1)%MAXSEM;
		}
		printf("%d:Golibroda goli klienta %d\n", (int)tmspec.tv_sec, clientid);
		buf.sem_num=rnr;
		buf.sem_op=1;
		semop(semphr, &buf, 1);
		printf("%d:Golibroda konczy strzyzenie klienta %d\n", (int)tmspec.tv_sec, clientid);
	}
	return 0;
}
