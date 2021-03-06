#define _GNU_SOURCE
#define UNIX_MAX_PATH 108

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
#include"header.h"

const int LOCAL = 0;
const int NET = 1;

char name[MAXNAMELEN];
char* host;
int type;
int port;
int sck;

void sighandler(int signo){
	struct message msg;
	msg.type=EXIT;
	send(sck, (void*)&msg, sizeof(struct message), 0);
	shutdown(sck, SHUT_RDWR);
	//close(sck);
	//unlink(name);
	exit(signo);
}

int main(int argc, char* argv[]){
	if(argc<4) return 1;
	strcpy(name, argv[1]);
	if(strcmp(argv[2], "net")==0) type = NET;
	else if (strcmp(argv[2], "local")==0) type = LOCAL;
	else return 1;
	if(type==LOCAL && strlen(argv[3])>UNIX_MAX_PATH) return 1;
	host=calloc(strlen(argv[3])+1, sizeof(char));
	strcpy(host, argv[3]);
	if(type==NET) port=atoi(argv[4]);
	else port=-1;
	struct message msg;
	struct sockaddr_in naddr;
	struct sockaddr_un laddr;
	if(type==NET){
		if((sck=socket(AF_INET, SOCK_DGRAM, 0))<0){
			perror("Creating socket");
			return errno;
		}
		//struct sockaddr_in addr;
		naddr.sin_family=AF_INET;
		naddr.sin_port=htons(port);
		inet_pton(AF_INET, host, &(naddr.sin_addr));
		/*if((bind(sck, (struct sockaddr*)&naddr, sizeof(struct sockaddr_un)))!=0){
			perror("Bind");
			return 1;
		}*/
		if(connect(sck, (struct sockaddr*)&naddr, sizeof(struct sockaddr_un))<0){
			perror("Connection");
			return errno;
		}
	}
	else {
		if((sck=socket(AF_UNIX, SOCK_DGRAM, 0))<0){
			perror("Creating socket");
			return errno;
		}
		//struct sockaddr_un addr;
		laddr.sun_family=AF_UNIX;
		//printf("%s\n", host);
		strcpy(laddr.sun_path, host);
		if((bind(sck, (struct sockaddr*)&laddr, sizeof(sa_family_t)))!=0){
			perror("Bind");
			return 1;
		}
		if((connect(sck, (struct sockaddr*)&laddr, sizeof(struct sockaddr_un)))!=0){
			perror("Connect");
			return 1;
		}
	}
	msg.type=HANDSHAKE;
	strcpy(msg.name, name);
	send(sck, (void*)&msg, sizeof(msg), 0);
	signal(SIGINT, sighandler);
	int size=0;
	char buf[1024];
	while(1){
		msg.type=0;
		size=recv(sck, buf, sizeof(struct message), 0);
		msg=*(struct message*)buf;
		if(size==0){
			printf("Server turned off\n");
			sighandler(0);
		}
		else if(msg.type==PING){
			send(sck, (void*)&msg, sizeof(struct message), 0);
		}
		else if(msg.type==MSG){
			msg.type=ANSWER;
			if (msg.sign=='+'){
				msg.answer = msg.num1+msg.num2;
			}else if (msg.sign=='-'){
				msg.answer = msg.num1-msg.num2;
			}else if (msg.sign=='*'){
				msg.answer = msg.num1*msg.num2;
			}else if (msg.sign=='/'){
				msg.answer = msg.num1/msg.num2;
			}
			printf("Dzialanie %d: %d\n", msg.cntr, msg.answer);
			send(sck, (void*)&msg, sizeof(struct message), 0);
		}
		else if(msg.type==DENIAL){
			sighandler(1);
		}
	}
	return 0;
}
