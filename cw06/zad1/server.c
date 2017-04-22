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
	queue = msgget(key, IPC_CREAT);
	if(queue==-1){
		printf("Can't create queue\n");
		exit(1);
	}
	time_t t = time(NULL);
	signal(SIGINT, finish);
	struct tm tm;
	struct clientbuf query;
	struct clientbuf response;
	response.mtype = ANSWER;
	query.mtext=calloc(1, sizeof(char));
	response.mtext=calloc(1, sizeof(char));
	while(1){
		msgrcv(queue, &query, MSGSIZE, 0, MSG_NOERROR);
		printf("Received message\n");
		if(query.mtype==ECHO){
			response.mtext = realloc(response.mtext,(strlen(query.mtext)+1)*sizeof(char));
			strcpy(response.mtext, query.mtext);
			msgsnd(query.qid, &response, MSGSIZE, MSG_NOERROR);
		}else if(query.mtype==WERS){
			response.mtext = realloc(response.mtext,(strlen(query.mtext)+1)*sizeof(char));
			response.mtext[strlen(query.mtext)]='\0';
			for(int i=0; i<strlen(query.mtext); i++){
				if((int)query.mtext[i]>=97&&(int)query.mtext[i]<=122){
					response.mtext[i]=query.mtext[i]-32;
				}
				else response.mtext[i]=query.mtext[i];
			}
			msgsnd(query.qid, &response, MSGSIZE, MSG_NOERROR);
		}else if(query.mtype==TIME){
			response.mtext = realloc(response.mtext,(17)*sizeof(char));
			tm = *localtime(&t);
			sprintf(response.mtext, "%d-%d-%d %d:%d", 
			tm.tm_mday, tm.tm_mon, tm.tm_year+1900, tm.tm_hour, tm.tm_min);
			msgsnd(query.qid, &response, MSGSIZE, MSG_NOERROR);
		}else if(query.mtype==STOP){
			msgctl(queue, IPC_RMID, NULL);
			exit(0);
		}
		printf("Processed message\n");
		//if(response.mtext!=NULL) free(response.mtext);
		//if(query.mtext!=NULL) free(query.mtext);
	}
	return 0;
}
