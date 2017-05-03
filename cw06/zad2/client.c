#include"header.h"

mqd_t server;
mqd_t client;
char path[16];

void finish(int signo){
	mq_close(server);
	mq_close(client);
	mq_unlink(path);
	exit(signo);
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
	ssize_t readBytes=0;
	char *line = calloc(100, sizeof(char));
	char *buf = calloc(100, sizeof(char));
	char msg[MAXSIZE];
	while(1){
		//fflush(stdout);
		printf("\n>>> ");
		fgets(line, 100, stdin);
		buf = strtok(line, " \n");
		if(strcmp(buf, "echo")==0){
			buf = strtok(NULL, "\n");
			sprintf(msg, "%s %s %s\n", "echo", path, buf);
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, msg, MAXSIZE, 0);
			mq_close(server);
			memset(msg, 0, MAXSIZE);
			//readBytes = mq_receive(client, msg, MAXSIZE, NULL);
			while((readBytes = mq_receive(client, msg, MAXSIZE, NULL))<=0){
				if(readBytes<0){
					printf("Cannot read from queue\n");
					perror(NULL);
					finish(1);
				}
				sleep(1);
			}
			printf("%s\n", msg);
		}else if(strcmp(buf, "wers")==0){
			buf = strtok(NULL, "\n");
			sprintf(msg, "%s %s %s\n", "wers", path, buf);
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, msg, MAXSIZE, 0);
			mq_close(server);
			memset(msg, 0, MAXSIZE);
			//readBytes = mq_receive(client, msg, MAXSIZE, NULL);
			while((readBytes = mq_receive(client, msg, MAXSIZE, NULL))<=0){
				if(readBytes<0){
					printf("Cannot read from queue\n");
					perror(NULL);
					finish(1);
				}
				sleep(1);
			}
			printf("%s\n", msg);
		}else if(strcmp(buf, "time")==0){
			sprintf(msg, "%s %s %s\n", "time", path, "-");
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, msg, MAXSIZE, 0);
			mq_close(server);
			memset(msg, 0, MAXSIZE);
			//readBytes = mq_receive(client, msg, MAXSIZE, NULL);
			while((readBytes = mq_receive(client, msg, MAXSIZE, NULL))<=0){
				if(readBytes<0){
					printf("Cannot read from queue\n");
					perror(NULL);
					finish(1);
				}
				sleep(1);
			}
			printf("%s\n", msg);
		}else if(strcmp(buf, "stop")==0){
			sprintf(msg, "%s %s %s\n", "stop", path, "-");
			server = mq_open(PROJECTQ, O_WRONLY);
			mq_send(server, msg, MAXSIZE, 0);
			mq_close(server);
		}else if(strcmp(buf, "exit")==0){
			finish(0);
		}
		memset(msg, 0, MAXSIZE);
	}
}
