#define _GNU_SOURCE
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
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

const int LOCAL = MAXCLIENTS-2;
const int NET = MAXCLIENTS-1;

int port;
int local;
int net;
int localclsck;
int netclsck;
struct sockaddr_in netaddr;
struct sockaddr_un localaddr;
struct pollfd monitor[MAXCLIENTS+2];
pthread_t funcs[3];
char *path;
int clients[MAXCLIENTS];
char clnames[MAXCLIENTS][MAXNAMELEN];
int cntr;
int pinged[MAXCLIENTS];

int getrandomclient(){
	int cntr=0;
	int tmp=0;
	for(int i=0; i<MAXCLIENTS; i++){
		if(monitor[i].fd!=-1) cntr++;
	}
	if(cntr==0) return -1;
	else if(cntr==1){
		for(int i=0; i<MAXCLIENTS; i++){
			if(monitor[i].fd!=-1) return i;
		}
	}
	else{
		tmp=rand()%cntr;
		cntr=0;
		for(int i=0; i<MAXCLIENTS; i++){
			if(monitor[i].fd!=-1){
				if(tmp==cntr++) return i;
			}
		}
	}
	return -1;
}

void sighandler(int signo){
	for(int i=0; i<3; i++) pthread_cancel(funcs[i]);
	for(int i=0; i<MAXCLIENTS+2; i++){
		if(monitor[i].fd!=-1){
			shutdown(monitor[i].fd, SHUT_RDWR);
			close(monitor[i].fd);
		}
	}
	unlink(path);
	exit(signo);
}


void* handlecommands(void* arg){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	char buf[64];
	char symbol;
	float num1;
	float num2;
	int ctr=0;
	int client=-1;
	struct message msg;
	while(1){
		while(fgets(buf, 64, stdin)!=NULL);
		if(sscanf(buf, "%f %c %f\n", &num1, &symbol, &num2)<=0) continue;
		if((symbol=='+'||symbol=='-'||symbol=='*'||symbol=='/')!=1) continue;
		client=getrandomclient();
		if(client==-1) printf("No clients connected\n");
		else{
			msg.type=MSG;
			msg.cntr=ctr++;
			msg.num1=num1;
			msg.num2=num2;
			msg.sign=symbol;
			write(monitor[client].fd, (void*)&msg, sizeof(msg));
		}
	}
	return NULL;
}
void* watch(void* arg){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	monitor[LOCAL].fd=local;
	monitor[NET].fd=net;
	for(int i=0; i<MAXCLIENTS; i++) monitor[i].fd=-1;
	for(int i=0; i<MAXCLIENTS+2; i++) monitor[i].events = POLLIN | POLLOUT | POLLRDHUP;
	listen(local, MAXCLIENTS);
	listen(net, MAXCLIENTS);
	int res;
	struct message buf;
	while(1){
		res=poll(monitor, MAXCLIENTS+2, 60000);
		if(res==0) break;
		for(int i=0; i<MAXCLIENTS+2; i++){
			if(i==LOCAL || i==NET){
				if(monitor[i].revents!=0){
					int placed=0;
					int handle = accept(monitor[i].fd, NULL, NULL);
					read(monitor[i].fd, (void*)&buf, sizeof(struct message));
					for(int j=0; j<MAXCLIENTS; j++){
						if(strcmp(clnames[j], buf.name)!=0 && monitor[j].fd==-1){
							placed=1;
							monitor[j].fd=handle;
							strcpy(clnames[j], buf.name);
							break;
						}
					}
					if(placed==0){
						shutdown(handle, SHUT_RDWR);
						close(handle);
					}
				}
			}
			else{
				if((monitor[i].revents & POLLRDHUP) != 0){
					shutdown(monitor[i].fd, SHUT_RDWR);
					close(monitor[i].fd);
					monitor[i].fd=-1;
					strcpy(clnames[i], "");
				}
				else if((monitor[i].revents & POLLIN) != 0){
					read(monitor[i].fd, (void*)&buf, sizeof(struct message));
					if(buf.type==PING) pinged[i]=1;
					else if(buf.type==ANSWER){
						//TODO
						printf("Odpowiedz na dzialanie nr %d: %f\n", buf.cntr, buf.answer);
					}
				}
			}
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
		if(monitor[i].fd!=-1){
			pinged[i]=0;
			write(monitor[i].fd, (void*)&msg, sizeof(struct message));
			sleep(1);
			if(pinged[i]==0){
				shutdown(monitor[i].fd, SHUT_RDWR);
				close(monitor[i].fd);
				monitor[i].fd=-1;
				strcpy(clnames[i], "");
			}
		}
		i=(i+1)%MAXCLIENTS;
	}
	return NULL;
}

int main(int argc, char* argv[]){
	if(argc<3) return 1;
	path=calloc(108, sizeof(char));
	//path = getcwd(path, 108);
	port=atoi(argv[1]);
	strcpy(path, argv[2]);
	signal(SIGINT, sighandler);
	cntr=0;
	for(int i=0; i<MAXCLIENTS; i++){
		strcpy(clnames[i], "");
	}
	if((local=socket(AF_UNIX, SOCK_STREAM, 0))<0){
		printf("Error in creating local socket\n");
		exit(1);
	}
	if((net=socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("Error in creating local socket\n");
		exit(1);
	}
	//memset((char*)&netaddr, '\0', sizeof(netaddr));
	netaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	netaddr.sin_port=htons(port);
	netaddr.sin_family=AF_INET;
	//memset((char*)&localaddr, '\0', sizeof(localaddr));
	localaddr.sun_family=AF_UNIX;
	strcpy(localaddr.sun_path, path);
	if(bind(local, (struct sockaddr*)&localaddr, sizeof(localaddr))<0){
		perror("Local bind");
		sighandler(errno);
	}
	if(bind(net, (struct sockaddr*)&netaddr, sizeof(netaddr))<0){
		perror("Net bind");
		sighandler(errno);
	}
	pthread_create(&funcs[0], NULL, handlecommands, NULL);
	pthread_create(&funcs[1], NULL, watch, NULL);
	pthread_create(&funcs[2], NULL, ping, NULL);
	
	
	unlink(path);
	return 0;
}
