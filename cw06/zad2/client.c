#include"header.h"

mqd_t serverq;
mqd_t client;
char path[16];

void finish(int signo){
	mq_close(serverq);
	mq_close(client);
	while(mq_unlink(path)==-1);
	exit(0);
}

int main(int argc, char *argv[]){
	path[0]='/';
	path[15]='\0';
	srand(time(NULL));
	struct mq_attr attr;
	attr.mq_flags=O_NONBLOCK;
	attr.mq_maxmsg = 20;
	attr.mq_msgsize = MSGSIZE;
	attr.mq_curmsgs=0;
	do{
		for(int i=1; i<15; i++){
			path[i]=(rand()%26)+97;
		}
	}while((client=mq_open(path, O_RDWR | O_CREAT | O_EXCL | O_NONBLOCK, 
	0777, &attr))==-1);
	signal(SIGINT, finish);
	char *line = calloc(100, sizeof(char));
	char *buf = calloc(100, sizeof(char));
	char msg[128];
	while(1){
		printf("\n>>> ");
		fgets(line, 128, stdin);
		buf = strtok(line, " \n\0");
		if(strcmp(buf, "echo")==0){
			strcat(msg, buf);
			strcat(msg, " ");
			strcat(msg, path);
			strcat(msg, " ");
			buf = strtok(NULL, "\n\0");
			strcat(msg, buf);
			serverq = mq_open(PROJECTQ, O_RDWR);
			mq_send(serverq, buf, 128, 1);
			mq_close(serverq);
			while(mq_receive(client, msg, 128, NULL)<=0);
			printf("%s\n", msg);
		}else if(strcmp(buf, "wers")==0){
			strcat(msg, buf);
			strcat(msg, " ");
			strcat(msg, path);
			strcat(msg, " ");
			buf = strtok(NULL, "\n\0");
			strcat(msg, buf);
			serverq = mq_open(PROJECTQ, O_RDWR);
			mq_send(serverq, buf, 128, 1);
			mq_close(serverq);
			while(mq_receive(client, msg, 128, NULL)<=0);
			printf("%s\n", msg);
		}else if(strcmp(buf, "time")==0){
			strcat(msg, buf);
			strcat(msg, " ");
			strcat(msg, path);
			strcat(msg, " ");
			buf = strtok(NULL, "\n\0");
			strcat(msg, buf);
			serverq = mq_open(PROJECTQ, O_RDWR);
			mq_send(serverq, buf, 128, 1);
			mq_close(serverq);
			while(mq_receive(client, msg, 128, NULL)<=0);
			printf("%s\n", msg);
		}else if(strcmp(buf, "stop")==0){
			strcat(msg, buf);
			strcat(msg, " ");
			strcat(msg, path);
			serverq = mq_open(PROJECTQ, O_RDWR);
			mq_send(serverq, buf, 128, 1);
			mq_close(serverq);
		}else if(strcmp(buf, "exit")==0){
			finish(NULL);
		}
	}
}
