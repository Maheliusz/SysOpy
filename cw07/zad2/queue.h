#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<semaphore.h>
#include<time.h>

static const int MAXSEM = 10;
//static const char* BARSEMID = "BARBERSEMAPHORE";
//static const char* SHMID = "SHAREDMEMORY";
//static const char* QSHMID = "QUEUESHM";


struct qnode{
	int pid;
	char semnum[8];
};

void generate_path(char *res);

void init_queue(struct qnode* node, int size);

struct qnode get_value(struct qnode *node, int size);

void add_to_queue(struct qnode in, struct qnode *node, int size);

int is_full(struct qnode* node, int size);

int queue_size(struct qnode* node, int size);

