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
#include<semaphore.h>
#include<time.h>

static const int MAXSEM = 10;


struct qnode{
	int pid;
	int semnum;
};

struct qnode get_value(struct qnode *node);

void add_to_queue(struct qnode in, struct qnode *node);

int is_full(struct qnode* node);

int queue_size(struct qnode* node);
