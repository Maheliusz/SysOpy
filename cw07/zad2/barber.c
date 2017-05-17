#include"queue.h"

sem_t *semphr;
sem_t *clientsem;
int shm;
unsigned int qlen;
int qshm;
pid_t ppid;
struct qnode *queue;
int *qlenshm;

void finish(int signo){
	printf("Golibroda konczy prace\n");
	sem_close(clientsem);
	sem_close(semphr);
	sem_unlink("/semaphore");
	munmap(queue, qlen*sizeof(struct qnode));
	munmap(qlenshm, 2*sizeof(int));
	close(shm);
	close(qshm);
	shm_unlink("/shm");
	shm_unlink("/qshm");
	exit(signo);
}

int main(int argc, char *argv[]){
	if(argc<2) return 1;
	qlen = atoi(argv[1]);
	qshm = shm_open("/qshm", O_CREAT|O_RDWR, 0777);
	ftruncate(qshm, 2*sizeof(int));
	qlenshm = (int *)mmap(NULL, 2*sizeof(int), 
		PROT_READ|PROT_WRITE|PROT_EXEC, MAP_SHARED, qshm, 0);
	//perror(NULL);
	qlenshm[0]=qlen;
	ppid = getpid();
	semphr = sem_open("/semaphore", O_CREAT, 0777, 0);
	//perror("Sem");
	shm = shm_open("/shm", O_CREAT|O_RDWR, 0777);
	ftruncate(shm, qlen*sizeof(struct qnode));
	//perror("Shm");
	queue=(struct qnode *)mmap(NULL, qlen*sizeof(struct qnode), 
		PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shm, 0);
	//perror("Q");
	init_queue(queue, qlen);
	signal(SIGINT, finish);
	struct qnode res;
	struct timespec tmspec;
	while(1){
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zasypia\n", (int)tmspec.tv_sec);
		sem_wait(semphr);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zostaje obudzony\n", (int)tmspec.tv_sec);
		res=get_value(queue, qlen);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda goli klienta %d\n", (int)tmspec.tv_sec, res.pid);
		clientsem = sem_open(res.semnum, 0);
		sem_post(clientsem);
		sem_close(clientsem);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda konczy strzyzenie klienta %d\n", (int)tmspec.tv_sec, res.pid);
	}
	return 0;
}
