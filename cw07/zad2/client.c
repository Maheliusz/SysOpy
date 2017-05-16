#include"queue.h"

sem_t *semphr;
sem_t *clientsem;
int shm;
pid_t ppid;
int qlen;
struct qnode *queue;
int *clients;
int isParent;
unsigned int clientcnt;

void fin_client(int signo){
	if(isParent){
		for(int i=0; i<clientcnt; i++) kill(clients[i], SIGINT);
	}
	else printf("Klient %d konczy prace\n", getpid());
	sem_close(semphr);
	munmap(queue, qlen*sizeof(struct qnode));
	close(shm);
	exit(signo);
}

int main(int argc, char *argv[]){
	if(argc<3) return 1;
	isParent=1;
	int qshm = shm_open("/qshm", O_RDWR, 0777);
	int *qlenshm = (int *)mmap(NULL, sizeof(int), 
		PROT_WRITE|PROT_EXEC|PROT_READ, MAP_SHARED, qshm, 0);
	qlen=qlenshm[0];
	munmap(qlenshm, sizeof(int));
	close(qshm);
	clientcnt = atoi(argv[1]);
	unsigned int cutcnt = atoi(argv[2]);
	shm = shm_open("/shm", O_RDWR, 0777);
	queue=(struct qnode *)mmap(NULL, qlen*sizeof(struct qnode), 
		PROT_READ | PROT_EXEC | PROT_WRITE, MAP_SHARED, shm, 0);
	signal(SIGINT, fin_client);
	struct timespec tmspec;
	pid_t pid;
	clients = calloc(clientcnt, sizeof(int));
	for(int i=0; i<clientcnt; i++){
		if((pid=fork())==0) break;
		clients[i]=pid;
	}
	if(pid==0){
		isParent=0;
		char name[8];
		generate_path(name);
		struct qnode node;
		node.pid=getpid();
		int cntr=0;
		while(cntr<cutcnt){
			clock_gettime(CLOCK_REALTIME, &tmspec);
			printf("%d:Klient %d wchodzi do poczekalni\n", 
				(int)tmspec.tv_sec, getpid());
			if(is_full(queue, qlen)==0){
				while((clientsem=sem_open(name, O_CREAT|O_EXCL, 0777, 0))==SEM_FAILED){
					generate_path(name);
				}
				strcpy(node.semnum, name);
				add_to_queue(node, queue, qlen);
				clock_gettime(CLOCK_REALTIME, &tmspec);
				printf("%d:Klient %d zajmuje miejsce w poczekalni, budzi golibrode\n",
					(int)tmspec.tv_sec, getpid());
				semphr = sem_open("/semaphore", 0);
				sem_post(semphr);
				sem_close(semphr);
				sem_wait(clientsem);
				clock_gettime(CLOCK_REALTIME, &tmspec);
				printf("%d:Klient %d zostal ostrzyzony %d/%d razy, ponownie staje w kolejce\n",
					(int)tmspec.tv_sec, getpid(), ++cntr, cutcnt);
				//zwolnij semafor
				sem_close(clientsem);
			}
			else{
				clock_gettime(CLOCK_REALTIME, &tmspec);
				printf("%d:Brak miejsc, klient %d opuszcza poczekalnie\n", 
					(int)tmspec.tv_sec, getpid());
			}
		}
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Klient %d zostal ostrzyzony %d/%d razy, opuszcza zaklad\n", 
			(int)tmspec.tv_sec, getpid(), cntr, cutcnt);
		fin_client(0);
	}
	//else wait(NULL);
	//fin_client(0);
	return 0;
}
