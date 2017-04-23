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
	char *line = NULL;
	char *buf = NULL;
	char *tmp = NULL;
	struct clientbuf query;
	struct clientbuf response;
	query.qid=privq;
	//line = calloc(1, sizeof(char));
	//buf = calloc(1, sizeof(char));
	while(1){
		line = realloc(line, 128*sizeof(char));
		tmp = realloc(line, 128*sizeof(char));
		buf = realloc(buf, 128*sizeof(char));
		printf("\n>>> ");
		fgets(line, 128, stdin);
		strcpy(tmp, line);
		buf = strtok(tmp, " \n\0");
		if(strcmp(buf, "echo")==0){
			query.mtype = ECHO;
			buf = strtok(NULL, "\n\0");
			query.mtext = realloc(query.mtext,(strlen(buf)+1)*sizeof(char));
			//query.mtext = buf;
			strcpy(query.mtext, buf);
			msgsnd(queue, &query, MSGSIZE, 0);
			//usleep(100);
			msgrcv(privq, (void*)&response, MSGSIZE, 0, 0);
			printf("%s\n", response.mtext);
		}else if(strcmp(buf, "wers")==0){
			query.mtype = WERS;
			buf = strtok(NULL, "\n\0");
			//query.mtext = realloc(query.mtext, (strlen(buf)+1)*sizeof(char));
			//strcpy(query.mtext, buf);
			query.mtext = buf;
			msgsnd(queue, &query, MSGSIZE, 0);
			//usleep(100);
			msgrcv(privq, (void*)&response, MSGSIZE, 0, 0);
			printf("\n%s\n", response.mtext);
		}else if(strcmp(buf, "time")==0){
			query.mtype = TIME;
			msgsnd(queue, &query, MSGSIZE, 0);
			//usleep(100);
			msgrcv(privq, (void*)&response, MSGSIZE, 0, 0);
			printf("\n%s\n", response.mtext);
		}else if(strcmp(buf, "stop")==0){
			query.mtype = STOP;
			msgsnd(queue, &query, MSGSIZE, 0);
		}else if(strcmp(buf, "exit")==0){
			msgctl(privq, IPC_RMID, NULL);
			exit(0);
		}
		
		//if(buf!=NULL) free(buf);
		//if(line!=NULL) free(line);
		//if(query.mtext!=NULL) free(query.mtext);
		//if(response.mtext!=NULL) free(response.mtext);
	}
	return 0;
}
