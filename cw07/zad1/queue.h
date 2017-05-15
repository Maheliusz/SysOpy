#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<time.h>

static const int MAXSEM = 10;


struct qnode{
	int pid;
	int semnum;
};

struct qnode get_value(struct qnode *node);

void init_queue(struct qnode* node);

void add_to_queue(struct qnode in, struct qnode *node);

int is_full(struct qnode* node);

int queue_size(struct qnode* node);

