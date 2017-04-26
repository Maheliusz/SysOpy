#include"header.h"

mqd_t server;
mqd_t client;

void finish(int signo){
	mq_close(server);
	mq_close(client);
	mq_unlink(PROJECTQ);
	exit(0);
}

void server_stop(){
	struct mq_attr attr;
	mq_getattr(server, &attr);
	if(attr.mq_curmsgs==0){
		mq_close(server);
		while(mq_unlink(PROJECTQ)==-1);
		exit(0);
	}
}

int main(int argc, char *argv[]){
	struct mq_attr attr;
	attr.mq_flags=0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAXSIZE;
	attr.mq_curmsgs=0;
	mq_unlink(PROJECTQ);
	server = mq_open(PROJECTQ, O_RDONLY | O_CREAT, 0644, &attr);
	if(server==-1){
		printf("Cannot create queue\n");
		exit(1);
	}
	ssize_t readBytes=0;
	time_t t;
	struct tm tmm;
	char line[MAXSIZE];
	char msg[MAXSIZE];
	char *buf = calloc(MAXSIZE, sizeof(char));
	char *addr = calloc(MAXSIZE, sizeof(char));
	int stop=0;
	signal(SIGINT, finish);
	while(1){
		while((readBytes=mq_receive(server, line, MAXSIZE, NULL))<=0){
			if(readBytes<0){
				printf("Cannot read from queue\n");
				perror(NULL);
				exit(1);
			}
		}
		//printf("%s\n", line);
		sscanf(line, "%s %s %s\n", buf, addr, msg);
		printf("Received message\n");
		if(strcmp(buf, "echo")==0){
			printf("Msg: %s\n", msg);
			client = mq_open(addr, O_WRONLY);
			mq_send(client, msg, 128, 0);
			mq_close(client);
		}else if(strcmp(buf, "wers")==0){
			printf("Msg: %s\n", msg);
			for(int i=0; i<128; i++){
				if((int)msg[i]>=97&&(int)msg[i]<=122) line[i]=msg[i]-32;
				else line[i]=msg[i];
				if(msg[i]=='\0') break;
			}
			client = mq_open(addr, O_WRONLY);
			mq_send(client, line, 128, 0);
			mq_close(client);
		}else if(strcmp(buf, "time")==0){
			t = time(NULL);
			tmm = *localtime(&t);
			strftime(buf, 128, "%c", &tmm);
			client = mq_open(addr, O_WRONLY);
			mq_send(client, msg, 128, 0);
			mq_close(client);
		}else if(strcmp(buf, "stop")==0){
			stop=1;
		}
		printf("Processed message\n");
		if(stop) server_stop();
	}
}
