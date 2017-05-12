#define _GNU_SOURCE
#define PROJECTQ "/my_queue"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<mqueue.h>
#include<time.h>
#include<signal.h>
#include<fcntl.h>

enum Type {ECHO = 1, WERS, TIME, STOP, ANSWER};

static const int MAXSIZE = 128;
