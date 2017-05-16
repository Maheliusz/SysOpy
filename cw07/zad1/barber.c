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
struct qnode *queue;
unsigned int qlen;
int qshm;
int *qlenshm;

union semun{
	int val;
};

void finish(int signo){
	printf("Golibroda konczy prace\n");
	semctl(semphr, 0, IPC_RMID, 0);
	shmdt(queue);
	shmctl(shm, IPC_RMID, NULL);
	shmdt(qlenshm);
	shmctl(qshm, IPC_RMID, NULL);
	exit(signo);
}

int main(int argc, char *argv[]){
	if(argc<2) return 1;
	qlen = atoi(argv[1]);
	ppid = getpid();
	//semphr = semget(ftok(getenv("HOME"), MAXSEM), MAXSEM+1, 0777 | IPC_CREAT);
	//shm = shmget(ftok(getenv("HOME"), MAXSEM), MAXSEM*sizeof(struct qnode), 0777 | IPC_CREAT);
	semphr = semget(ftok(getenv("HOME"), MAXSEM), qlen+1, 0777 | IPC_CREAT);
	shm = shmget(ftok(getenv("HOME"), MAXSEM), qlen*sizeof(struct qnode), 0777 | IPC_CREAT);
	qshm = shmget(ftok(getenv("HOME"), 0), sizeof(int), 0777 | IPC_CREAT);
	queue = (struct qnode*)shmat(shm, NULL, 0);
	qlenshm = (int *)shmat(qshm, NULL, 0);
	qlenshm[0]=qlen;
	init_queue(queue, qlen);
	union semun attr;
	attr.val=1;
	//for(int i=1; i<MAXSEM+1; i++)semctl(semphr, i, SETVAL, attr);
	for(int i=1; i<qlen+1; i++)semctl(semphr, i, SETVAL, attr);
	attr.val=0;
	semctl(semphr, 0, SETVAL, attr);
	signal(SIGINT, finish);
	struct timespec tmspec;
	struct sembuf buf;
	signal(SIGINT, finish);
	struct qnode res;
	while(1){
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zasypia\n", (int)tmspec.tv_sec);
		buf.sem_num=0;
		buf.sem_op=-1;
		semop(semphr, &buf, 1);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zostaje obudzony\n", (int)tmspec.tv_sec);
		sleep(1);
		res=get_value(queue, qlen);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda goli klienta %d\n", (int)tmspec.tv_sec, res.pid);
		buf.sem_num=res.semnum;
		buf.sem_op=1;
		semop(semphr, &buf, 1);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda konczy strzyzenie klienta %d\n", (int)tmspec.tv_sec, res.pid);
	}
	return 0;
}
