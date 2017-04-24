#include "header.h"

int queue;

void finish(int signo){
	msgctl(queue, IPC_RMID, NULL);
	exit(0);
}

int main(int argc, char *argv[]){
	//char *path = calloc(128, sizeof(char));
	//path = getenv("HOME");
	//key_t key = ftok(getenv("HOME"), PROJECTID);
	queue = msgget(ftok(getenv("HOME"), PROJECTID), 0777);
	signal(SIGINT, finish);
	int privq;
	if((privq = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0777))==-1){
		printf("Private queue cannot be created\n");
		exit(1);
	}
	if(queue==-1){
		printf("Can't connect to server\n");
		exit(1);
	}
	char *line = calloc(128, sizeof(char));
	char *buf = calloc(128, sizeof(char));
	struct clientbuf query;
	//struct clientbuf response;
	query.qid=privq;
	while(1){
		printf("\n>>> ");
		fgets(line, 128, stdin);
		buf = strtok(line, " \n\0");
		if(strcmp(buf, "echo")==0){
			query.mtype = ECHO;
			buf = strtok(NULL, "\n\0");
			//query.mtext = buf;
			strncpy(query.mtext, buf, strlen(buf));
			//printf("Arg:%s\n", query.mtext);
			query.mtext[strlen(buf)]='\0';
			msgsnd(queue, &query, MSGSIZE, 0);
			//usleep(100);
			msgrcv(privq, &query, MSGSIZE, 0, 0);
			printf("%s\n", query.mtext);
		}else if(strcmp(buf, "wers")==0){
			query.mtype = WERS;
			buf = strtok(NULL, "\n\0");
			strncpy(query.mtext, buf, strlen(buf));
			query.mtext[strlen(buf)]='\0';
			//printf("Arg:%s\n", query.mtext);
			msgsnd(queue, &query, MSGSIZE, 0);
			//usleep(100);
			msgrcv(privq, &query, MSGSIZE, 0, 0);
			printf("\n%s\n", query.mtext);
		}else if(strcmp(buf, "time")==0){
			query.mtype = TIME;
			msgsnd(queue, &query, MSGSIZE, 0);
			//usleep(100);
			msgrcv(privq, &query, MSGSIZE, 0, 0);
			printf("\n%s\n", query.mtext);
		}else if(strcmp(buf, "stop")==0){
			query.mtype = STOP;
			msgsnd(queue, &query, MSGSIZE, 0);
		}else if(strcmp(buf, "exit")==0){
			msgctl(privq, IPC_RMID, NULL);
			exit(0);
		}
	}
	return 0;
}
