#include "header.h"

int queue;

void finish(int signo){
	msgctl(queue, IPC_RMID, NULL);
	exit(0);
}

int main(int argc, char *argv[]){
	char *path = calloc(128, sizeof(char));
	path = getenv("HOME");
	key_t key = ftok(path, PROJECTID);
	queue = msgget(key, 0);
	signal(SIGINT, finish);
	int privq;
	if((privq = msgget(IPC_PRIVATE, IPC_CREAT | 0777))==-1){
		printf("Private queue cannot be created\n");
		exit(1);
	}
	if(queue==-1){
		printf("Can't connect to server\n");
		exit(1);
	}
	char *line;
	char *buf;
	struct clientbuf query;
	struct clientbuf response;
	query.qid=privq;
	line = calloc(1, sizeof(char));
	buf = calloc(1, sizeof(char));
	while(1){
		line = realloc(line, 128*sizeof(char));
		buf = realloc(buf, 128*sizeof(char));
		printf("\n>>>");
		fgets(line, 128, stdin);
		buf = strtok(line, " \n");
		if(strcmp(buf, "echo")==0){
			query.mtype = ECHO;
			buf = strtok(NULL, "\n");
			query.mtext = realloc(query.mtext,(strlen(buf)+1)*sizeof(char));
			strcpy(query.mtext, buf);
			msgsnd(queue, &query, MSGSIZE, MSG_NOERROR);
			msgrcv(privq, &response, MSGSIZE, 0, MSG_NOERROR);
			printf("\n%s\n", response.mtext);
		}else if(strcmp(buf, "wers")==0){
			query.mtype = WERS;
			buf = strtok(NULL, "\n");
			query.mtext = realloc(query.mtext, (strlen(buf)+1)*sizeof(char));
			strcpy(query.mtext, buf);
			msgsnd(queue, &query, MSGSIZE, MSG_NOERROR);
			msgrcv(privq, &response, MSGSIZE, 0, MSG_NOERROR);
			printf("\n%s\n", response.mtext);
		}else if(strcmp(buf, "time")==0){
			query.mtype = TIME;
			msgsnd(queue, &query, MSGSIZE, MSG_NOERROR);
			msgrcv(privq, &response, MSGSIZE, 0, MSG_NOERROR);
			printf("\n%s\n", response.mtext);
		}else if(strcmp(buf, "stop")==0){
			query.mtype = STOP;
			msgsnd(queue, &query, MSGSIZE, MSG_NOERROR);
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
