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
char path[UNIX_MAX_PATH];
int clients[MAXCLIENTS];
char clnames[MAXCLIENTS][MAXNAMELEN];
int cntr;
int pinged[MAXCLIENTS];

int getrandomclient(){
	int c=0;
	int tmp=0;
	for(int i=0; i<MAXCLIENTS; i++){
		if(monitor[i].fd!=-1) c++;
	}
	if(c==0) return -1;
	else if(c==1){
		for(int i=0; i<MAXCLIENTS; i++){
			if(monitor[i].fd!=-1) return i;
		}
	}
	else{
		tmp=rand()%c;
		c=0;
		for(int i=0; i<MAXCLIENTS; i++){
			if(monitor[i].fd!=-1){
				if(tmp==c++) return i;
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
			msg.cntr=cntr++;
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
	for(int i=0; i<MAXCLIENTS; i++) monitor[i].fd=-1;
	for(int i=0; i<MAXCLIENTS+2; i++) monitor[i].events = POLLIN | POLLOUT | POLLRDHUP;
	listen(monitor[LOCAL].fd, MAXCLIENTS);
	listen(monitor[NET].fd, MAXCLIENTS);
	int res;
	struct message msg;
	char buf[1024];
	while(1){
		res=poll(monitor, MAXCLIENTS+2, 60000);
		if(res==0) break;
		for(int i=0; i<MAXCLIENTS+2; i++){
			if(i==LOCAL || i==NET){
				if(monitor[i].revents!=0){
					int placed=0;
					int handle = accept(monitor[i].fd, NULL, NULL);
					read(monitor[i].fd, buf, sizeof(struct message));
					msg=*(struct message*)buf;
					for(int j=0; j<MAXCLIENTS; j++){
						if(strcmp(clnames[j], msg.name)!=0 && monitor[j].fd==-1){
							placed=1;
							monitor[j].fd=handle;
							strcpy(clnames[j], msg.name);
							break;
						}
					}
					if(placed==0){
						msg.type=DENIAL;
						write(monitor[i].fd, (void*)&msg, sizeof(struct message));
						shutdown(handle, SHUT_RDWR);
						close(handle);
					}
				}
			}
			else{
				if((monitor[i].revents & (POLLRDHUP|POLLHUP)) != 0){
					shutdown(monitor[i].fd, SHUT_RDWR);
					close(monitor[i].fd);
					monitor[i].fd=-1;
					strcpy(clnames[i], "");
				}
				else if((monitor[i].revents & POLLIN) != 0){
					read(monitor[i].fd, buf, sizeof(struct message));
					msg=*(struct message*)buf;
					if(msg.type==PING) pinged[i]=1;
					else if(msg.type==ANSWER){
						printf("Odpowiedz na dzialanie nr %d: %f\n", msg.cntr, msg.answer);
					}
					else if(msg.type==EXIT){
						shutdown(monitor[i].fd, SHUT_RDWR);
						close(monitor[i].fd);
						monitor[i].fd=-1;
						strcpy(clnames[i], "");
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
	//path=calloc(108, sizeof(char));
	//path = getcwd(path, 108);
	port=atoi(argv[1]);
	strcpy(path, argv[2]);
	signal(SIGINT, sighandler);
	cntr=0;
	for(int i=0; i<MAXCLIENTS; i++){
		strcpy(clnames[i], "");
	}
	if((monitor[LOCAL].fd=socket(AF_UNIX, SOCK_STREAM, 0))<0){
		printf("Error in creating local socket\n");
		exit(1);
	}
	if((monitor[NET].fd=socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("Error in creating local socket\n");
		exit(1);
	}
	netaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	netaddr.sin_port=htons(port);
	netaddr.sin_family=AF_INET;
	localaddr.sun_family=AF_UNIX;
	strcpy(localaddr.sun_path, path);
	if(bind(monitor[LOCAL].fd, (struct sockaddr*)&localaddr, sizeof(struct sockaddr))<0){
		perror("Local bind");
		sighandler(errno);
	}
	if(bind(monitor[NET].fd, (struct sockaddr*)&netaddr, sizeof(struct sockaddr))<0){
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
