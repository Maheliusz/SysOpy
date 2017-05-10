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
int queue[MAXSEM];

union semun{
	int val;
};

void finish(int signo){
	clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda konczy prace\n", tmspec.tv_sec);
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
	attr.val=0;
	for(int i=0; i<MAXSEM; i++)semctl(semphr, i, SETVAL, attr);
	pid_t pid;
	int clientid;
	for(int i=0; i<clientcnt; i++){
		if((pid=fork())==0){
			clientid=i;
			clients[i]=pid;
			break;
		}
		else if(pid<0){
			printf("Forking problem\n");
			finish(1);
		}
		clients[clientrnr++]=pid;
		signal(SIGINT, finish);
	}
	struct timespec tmspec;
	if(pid==0){
		buf = struct sembuf;
		buf.sem_flg=0;
		int cntr=0;
		int place;
		while(cntr<cutcnt){
			clock_gettime(CLOCK_REALTIME, &tmspec);
			printf("%d:Klient %d wchodzi do poczekalni, budzi golibrode\n", 
				tmspec.tv_sec, getpid());
			kill(ppid, SIGALRM);
			place=-1;
			for(int i=0; i<MAXSEM; i++){
				if(semctl(semphr, i, GETVAL)==1){
					place=i;
					queue[place]=getpid();
					clock_gettime(CLOCK_REALTIME, &tmspec);
					printf("%d:Klient %d zajmuje %d miejsce w poczekalni\n",
						tmspec.tv_sec, getpid(), i);
					buf.sem_num=i;
					buf.sem_op=-1;
					semop(semphr, &buf, 1);
					printf("%d:Klient %d zostal ostrzyzony %d raz, ponownie staje w kolejce\n",
						tmspec.tv_sec, getpid(), ++cntr);
					queue[place]=-1;
				}
			}
			if(place==-1){
				clock_gettime(CLOCK_REALTIME, &tmspec);
				printf("%d:Brak miejsc, klient %d opuszcza poczekalnie\n", 
					tmspec.tv_sec, getpid());
			}
		}
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Klient %d zostal ostrzyzony %d/%d razy, opuszcza zaklad\n", 
			tmspec.tv_sec, getpid(), cntr, cutcnt);
		exit(0);
	}else{
		int rnr=0;
		int clientid;
		signal(SIGINT, finish);
		buf.sem_op=1;
		while(true){
			clock_gettime(CLOCK_REALTIME, &tmspec);
			printf("%d:Golibroda zasypia\n", tmspec.tv_sec);
			pause();
			clock_gettime(CLOCK_REALTIME, &tmspec);
			printf("%d:Golibroda zostaje obudzony\n", tmspec.tv_sec);
			while((clientid=queue[rnr])==-1){
				rnr=(rnr+1)%MAXSEM;
			}
			printf("%d:Golibroda goli klienta %d\n", tmspec.tv_sec, clientid);
			semop(semphr, &buf, 1);
			printf("%d:Golibroda konczy strzyzenie klienta %d\n", tmspec.tv_sec, clientid);
		}
	}
	return 0;
}
