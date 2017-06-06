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

int port;
int local;
int net;
int localsck;
int netsck;
struct sockaddr_in netaddr;
struct sockaddr_un localaddr;
struct pollfd monitor[2];
struct sockaddr clientaddr[MAXCLIENTS];
int connection[MAXCLIENTS];
pthread_t funcs[3];
char path[UNIX_MAX_PATH];
int clients[MAXCLIENTS];
char clnames[MAXCLIENTS][MAXNAMELEN];
int cntr;
int pinged[MAXCLIENTS];

int getrandomclient(){
	int c=0;
	int tmp=0;
	for(int i=0; i<MAXCLIENTS; i++){
		if(strcmp(clnames[i], "")!=0) c++;
	}
	if(c==0) return -1;
	else if(c==1){
		for(int i=0; i<MAXCLIENTS; i++){
			if(strcmp(clnames[i], "")!=0) return i;
		}
	}
	else{
		tmp=rand()%c;
		c=0;
		for(int i=0; i<MAXCLIENTS; i++){
			if(strcmp(clnames[i], "")!=0){
				if(tmp==c++) return i;
			}
		}
	}
	return -1;
}

void sighandler(int signo){
	for(int i=0; i<3; i++) pthread_cancel(funcs[i]);
	unlink(path);
	close(monitor[NET].fd);
	close(monitor[LOCAL].fd);
	exit(signo);
}


void* handlecommands(void* arg){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	char buf[64];
	char symbol;
	//char* tmp;
	char num1[25];
	char num2[25];
	int i, j;
	int client=-1;
	struct message msg;
	while(1){
		fgets(buf, 64, stdin);
		for(i=0; i<strlen(buf)&&buf[i]>='0'&&buf[i]<='9'; i++)num1[i]=buf[i];
		num1[i-1]='\0';
		symbol=buf[i++];
		for(j=i; j<strlen(buf)&&buf[j]>='0'&&buf[j]<='9'; j++)num2[j-i-1]=buf[j];
		num2[j-i-1]='\0';
		if((symbol=='+'||symbol=='-'||symbol=='*'||symbol=='/')!=1) continue;
		client=getrandomclient();
		if(client==-1) printf("No clients connected\n");
		else{
			msg.type=MSG;
			msg.cntr=cntr++;
			msg.num1=atoi(num1);
			msg.num2=atoi(num2);
			msg.sign=symbol;
			//TODO
			if(connection[client]==NET)	sendto(monitor[NET].fd, (void*)&msg, sizeof(msg), 0, &clientaddr[i], sizeof(struct sockaddr));
			else sendto(monitor[LOCAL].fd, (void*)&msg, sizeof(msg), 0, &clientaddr[i], sizeof(struct sockaddr));
		}
	}
	return NULL;
}
void handlemsg(struct message msg, struct sockaddr client, int sck){
	if(msg.type==HANDSHAKE){
		int taken=0;
		int placed=0;
		for(int i=0; i<MAXCLIENTS; i++){
			if(strcmp(msg.name, clnames[i])==0){ 
				taken=1;
				break;
			}
		}
		for(int i=0; i<MAXCLIENTS; i++){
			if(taken) break;
			if(strcmp(clnames[i], "")==0){
				placed=1;
				memcpy((void*)&clients[i], (void*)&client, sizeof(struct sockaddr));
				strcpy(clnames[i], msg.name);
				connection[i]=NET;
			}
		}
		if(placed==0){
			msg.type=DENIAL;
			sendto(sck, (void*)&msg, sizeof(struct message), 0, &client, sizeof(struct sockaddr));
		}
	}
	else if(msg.type==ANSWER){
		printf("Odpowiedz na dzialanie nr %d: %d\n", msg.cntr, msg.answer);
	}
	else if(msg.type==PING){
		for(int i=0; i<MAXCLIENTS; i++){
			if(strcmp(msg.name, clnames[i])==0){
				pinged[i]=0;
			}
		}
	}
	else if(msg.type==EXIT){
		for(int i=0; i<MAXCLIENTS; i++){
			if(strcmp(msg.name, clnames[i])==0){
				pinged[i]=0;
				strcpy(clnames[i], "");
				connection[i]=-1;
			}
		}
	}
}
void* watch(void* arg){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	struct message msg;
	char buf[1024];
	struct sockaddr client;
	while(1){
		//TODO
		monitor[LOCAL].events=POLLIN|POLLOUT;
		monitor[NET].events=POLLIN|POLLOUT;
		poll(monitor, 2, -1);
		if(monitor[LOCAL].revents!=0){
			recvfrom(monitor[LOCAL].fd, (void*)&buf, sizeof(struct message), 0, &client, NULL);
			msg = *(struct message*)buf;
			handlemsg(msg, client, monitor[LOCAL].fd);
		} else if(monitor[LOCAL].revents!=0){
			recvfrom(monitor[NET].fd, (void*)&buf, sizeof(struct message), 0, &client, NULL);
			msg = *(struct message*)buf;
			handlemsg(msg, client, monitor[NET].fd);
		}
	}
	return NULL;
}

void* ping(void* arg){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int i=0;
	struct message msg;
	msg.type=PING;
	while(1){
		if(strcmp(clnames[i], "")!=0){
			pinged[i]=1;
			if(connection[i]==NET) sendto(netsck, (void*)&msg, sizeof(struct message), 0, &clientaddr[i], sizeof(struct sockaddr));
			else sendto(localsck, (void*)&msg, sizeof(struct message), 0, &clientaddr[i], sizeof(struct sockaddr));
			sleep(1);
			if(pinged[i]){
				strcpy(clnames[i], "");
				connection[i]=-1;
			}
		}
		i=(i+1)%MAXCLIENTS;
	}
	return NULL;
}

int main(int argc, char* argv[]){
	if(argc<3) return 1;
	//path=calloc(108, sizeof(char));
	//path = getcwd(path, 108);
	port=atoi(argv[1]);
	strcpy(path, argv[2]);
	signal(SIGINT, sighandler);
	cntr=0;
	for(int i=0; i<MAXCLIENTS; i++){
		strcpy(clnames[i], "");
		connection[i]=-1;
	}
	if((monitor[LOCAL].fd=socket(AF_UNIX, SOCK_DGRAM, 0))<0){
		printf("Error in creating local socket\n");
		exit(1);
	}
	if((monitor[NET].fd=socket(AF_INET, SOCK_DGRAM, 0))<0){
		printf("Error in creating local socket\n");
		exit(1);
	}
	netaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	netaddr.sin_port=htons(port);
	netaddr.sin_family=AF_INET;
	localaddr.sun_family=AF_UNIX;
	strcpy(localaddr.sun_path, path);
	if(bind(monitor[LOCAL].fd, (struct sockaddr*)&localaddr, sizeof(struct sockaddr_un))<0){
		perror("Local bind");
		sighandler(errno);
	}
	if(bind(monitor[NET].fd, (struct sockaddr*)&netaddr, sizeof(struct sockaddr_in))<0){
		perror("Net bind");
		sighandler(errno);
	}
	pthread_create(&funcs[0], NULL, handlecommands, NULL);
	pthread_create(&funcs[1], NULL, watch, NULL);
	pthread_create(&funcs[2], NULL, ping, NULL);
	for(int i=0; i<3; i++){
		pthread_join(funcs[i], NULL);
	}	
	unlink(path);
	return 0;
}
