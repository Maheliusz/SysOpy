#include"queue.h"

sem_t *semphr;
sem_t *clientsem;
int shm;
pid_t ppid;
struct qnode *queue;

void finish(int signo){
	printf("Golibroda konczy prace\n");
	sem_close(semphr);
	sem_unlink(getenv("HOME"));
	munmap(queue, sizeof(MAXSEM*sizeof(struct qnode)));
	close(shm);
	shm_unlink(getenv("HOME"));
	exit(signo);
}

int main(int argc, char *argv[]){
	ppid = getpid();
	semphr = sem_open(getenv("HOME"), IPC_CREAT, 0777, 0);
	shm = shm_open(getenv("HOME"), O_RDWR | O_CREAT, 0777);
	queue=(struct qnode *)mmap(NULL, sizeof(MAXSEM*sizeof(struct qnode)), 
		PROT_READ | PROT_EXEC | PROT_WRITE, MAP_SHARED, shm, 0);
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
