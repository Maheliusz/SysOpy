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
	sem_unlink(getenv("HOME"));
	munmap(queue, qlen*sizeof(struct qnode));
	munmap(qlenshm, sizeof(int));
	close(shm);
	close(qshm);
	shm_unlink(getenv("HOME"));
	exit(signo);
}

int main(int argc, char *argv[]){
	if(argc<2) return 1;
	qlen = atoi(argv[1]);
	qshm = shm_open(getenv("PWD"), O_WRONLY | O_CREAT, 0777);
	qlenshm = (int *)mmap(NULL, sizeof(int), 
		PROT_EXEC | PROT_WRITE, MAP_SHARED, qshm, 0);
	qlenshm[0]=qlen;
	ppid = getpid();
	semphr = sem_open(getenv("HOME"), O_CREAT, 0777, 0);
	//perror("Sem");
	shm = shm_open(getenv("HOME"), O_RDWR | O_CREAT, 0777);
	//perror("Shm");
	queue=(struct qnode *)mmap(NULL, qlen*sizeof(struct qnode), 
		PROT_READ | PROT_EXEC | PROT_WRITE, MAP_SHARED, shm, 0);
	//perror("Q");
	signal(SIGINT, finish);
	struct qnode res;
	struct timespec tmspec;
	while(1){
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zasypia\n", (int)tmspec.tv_sec);
		sem_wait(semphr);
		clock_gettime(CLOCK_REALTIME, &tmspec);
		printf("%d:Golibroda zostaje obudzony\n", (int)tmspec.tv_sec);
		res=get_value(queue);
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
