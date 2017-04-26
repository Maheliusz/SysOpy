#include"header.h"

mqd_t server;
mqd_t client;
char path[16];

void finish(int signo){
	mq_close(server);
	mq_close(client);
	mq_unlink(path);
	exit(0);
}

int main(int argc, char *argv[]){
	path[0]='/';
	srand(time(NULL));
	struct mq_attr attr;
	attr.mq_flags=0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAXSIZE;
	attr.mq_curmsgs=0;
	do{
		for(int i=1; i<16; i++){
			path[i]=(char)((rand()%26)+'a');
		}
	}while((client=mq_open(path, O_RDONLY | O_CREAT, 0644, &attr))==-1);
	signal(SIGINT, finish);
	char *line = calloc(100, sizeof(char));
	char *buf = calloc(100, sizeof(char));
	char msg[MAXSIZE];
	while(1){
		printf("\n>>> ");
		fgets(line, MAXSIZE, stdin);
		buf = strtok(line, " \n");
		if(strcmp(buf, "echo")==0){
			buf = strtok(NULL, "\n");
			sprintf(buf, "%s %s %s\n", "echo", path, buf);
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, buf, MAXSIZE, 0);
			mq_close(server);
			while(mq_receive(client, msg, MAXSIZE, NULL)<=0);
			printf("%s\n", msg);
		}else if(strcmp(buf, "wers")==0){
			buf = strtok(NULL, "\n");
			sprintf(buf, "%s %s %s\n", "echo", path, buf);
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, buf, MAXSIZE, 0);
			mq_close(server);
			while(mq_receive(client, msg, MAXSIZE, NULL)<=0);
			printf("%s\n", msg);
		}else if(strcmp(buf, "time")==0){
			sprintf(buf, "%s %s\n", "time", path);
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, buf, MAXSIZE, 0);
			mq_close(server);
			while(mq_receive(client, msg, MAXSIZE, NULL)<=0);
			printf("%s\n", msg);
		}else if(strcmp(buf, "stop")==0){
			sprintf(buf, "%s %s\n", "stop", path);
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, buf, MAXSIZE, 0);
			mq_close(server);
		}else if(strcmp(buf, "exit")==0){
			finish(0);
		}
	}
}
