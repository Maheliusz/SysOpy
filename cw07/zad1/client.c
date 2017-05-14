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
struct qnode *queue;
int *clients;

void fin_client(int signo){
	printf("Klient %d konczy prace\n", getpid());
	shmdt(queue);
	exit(signo);
}

int main(int argc, char *argv[]){
	if(argc<3) return 1;
	unsigned int clientcnt = atoi(argv[1]);
	unsigned int cutcnt = atoi(argv[2]);
	clients = calloc(clientcnt, sizeof(int));
	semphr = semget(ftok(getenv("HOME"), MAXSEM), MAXSEM+1, 0);
	shm = shmget(ftok(getenv("HOME"), MAXSEM), MAXSEM*sizeof(struct qnode), 0);
	queue = (struct qnode *)shmat(shm, NULL, 0);
	signal(SIGINT, fin_client);
	struct timespec tmspec;
	struct sembuf buf;
	pid_t pid;
	for(int i=0; i<clientcnt; i++){
		if((pid=fork())==0) break;
	}
	if(pid==0){
		struct qnode node;
		node.pid=getpid();
		buf.sem_flg=0;
		int cntr=0;
		int place;
		while(cntr<cutcnt){
			clock_gettime(CLOCK_REALTIME, &tmspec);
			printf("%d:Klient %d wchodzi do poczekalni\n", 
				(int)tmspec.tv_sec, getpid());
			place=-1;
			for(int i=1; i<=MAXSEM; i++){
				if(semctl(semphr, i, GETVAL)==1){
					buf.sem_num=i;
					buf.sem_op=-1;
					semop(semphr, &buf, 1);
					place=i;
					node.semnum=i;
					add_to_queue(node, queue);
					clock_gettime(CLOCK_REALTIME, &tmspec);
					printf("%d:Klient %d zajmuje %d miejsce w poczekalni, budzi golibrode\n",
						(int)tmspec.tv_sec, getpid(), i);
					buf.sem_num=0;
					buf.sem_op=1;
					semop(semphr, &buf, 1);
					buf.sem_num=i;
					buf.sem_op=-1;
					semop(semphr, &buf, 1);
					clock_gettime(CLOCK_REALTIME, &tmspec);
					printf("%d:Klient %d zostal ostrzyzony %d/%d razy, ponownie staje w kolejce\n",
					(int)tmspec.tv_sec, getpid(), ++cntr, cutcnt);
					buf.sem_num=i;
					buf.sem_op=1;
					semop(semphr, &buf, 1);
					break;
				}
			}
			if(place==-1){
				clock_gettime(CLOCK_REALTIME, &tmspec);
				printf("%d:Brak miejsc, klient %d opuszcza poczekalnie\n", 
					(int)tmspec.tv_sec, getpid());
			}
		}
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Klient %d zostal ostrzyzony %d/%d razy, opuszcza zaklad\n", 
			(int)tmspec.tv_sec, getpid(), cntr, cutcnt);
	}
	//else wait(NULL);
	fin_client(0);
	return 0;
}
