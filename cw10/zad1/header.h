#define _GNU_SOURCE
#define UNIX_MAX_PATH 108
#define MAXCLIENTS 16
#define MAXNAMELEN 16


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<endian.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<limits.h>
#include<poll.h>
#include<pthread.h>

enum type{
	PING = 20, MSG, ANSWER, HANDSHAKE, EXIT, DENIAL
};
enum op{
	ADD = 30, SUB, MUL, DIV
};

struct message{
	enum type type;
	float answer;
	//struct data stuff;
	int cntr;
	float num1;
	float num2;
	char sign;
	char name[MAXNAMELEN];	
};
