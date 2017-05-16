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

struct qnode get_value(struct qnode *node, int size);

void init_queue(struct qnode* node, int size);

void add_to_queue(struct qnode in, struct qnode *node, int size);

int is_full(struct qnode* node, int size);

int queue_size(struct qnode* node, int size);

