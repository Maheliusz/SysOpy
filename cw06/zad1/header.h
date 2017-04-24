#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<time.h>
#include<signal.h>
#include<fcntl.h>

struct clientbuf{
	long mtype;
	char mtext[128];
	int qid;
};

enum Type {ECHO = 1, WERS, TIME, STOP, ANSWER};

static const char PROJECTID = 'a';
static const int MSGSIZE = 4096;
