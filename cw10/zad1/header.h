#define _GNU_SOURCE
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
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


//ponizsze wartosci nie maja znaczenia
//static const int NET = 12;
//static const int LOCAL = 13;
static const int PING = 20;
static const int MSG = 21;
static const int ANSWER = 22;
static const int HANDSHAKE = 23;

/*struct data{
	int cntr;
	int num1;
	int num2;
	char sign;
};*/

struct message{
	int type;
	float answer;
	//struct data stuff;
	int cntr;
	int num1;
	int num2;
	char sign;
	char name[MAXNAMELEN];	
};
