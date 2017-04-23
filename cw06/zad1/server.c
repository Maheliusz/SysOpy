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
	queue = msgget(ftok(getenv("HOME"), PROJECTID), IPC_CREAT | 0777);
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
	query.mtext=NULL;
	response.mtext=NULL;
	//query.mtext=calloc(1, sizeof(char));
	//response.mtext=calloc(1, sizeof(char));
	while(1){
		while(msgrcv(queue, (void*)&query, MSGSIZE, 0, 0)==-1);
		printf("Received message\n");
		if(query.mtype==ECHO){
			response.mtext = realloc(response.mtext,(strlen(query.mtext)+1)*sizeof(char));
			strncpy(response.mtext, query.mtext, strlen(query.mtext));
			//usleep(100);
			printf("Echo\n");
			msgsnd(query.qid, &response, MSGSIZE, 0);
		}else if(query.mtype==WERS){
			response.mtext = realloc(response.mtext,(strlen(query.mtext)+1)*sizeof(char));
			response.mtext[strlen(query.mtext)]='\0';
			for(int i=0; i<strlen(query.mtext); i++){
				if((int)query.mtext[i]>=97&&(int)query.mtext[i]<=122){
					response.mtext[i]=query.mtext[i]-32;
				}
				else response.mtext[i]=query.mtext[i];
			}
			printf("Changing text\n");
			//usleep(100);
			msgsnd(query.qid, &response, MSGSIZE, 0);
		}else if(query.mtype==TIME){
			response.mtext = realloc(response.mtext,(17)*sizeof(char));
			tm = *localtime(&t);
			sprintf(response.mtext, "%d-%d-%d %d:%d", 
			tm.tm_mday, tm.tm_mon, tm.tm_year+1900, tm.tm_hour, tm.tm_min);
			//usleep(100);
			printf("Sending time\n");
			msgsnd(query.qid, &response, MSGSIZE, 0);
		}else if(query.mtype==STOP){
			//usleep(100);	
			msgctl(queue, IPC_RMID, NULL);
			printf("Stopping server\n");
			exit(0);
		}
		printf("Processed message\n");
		//if(response.mtext!=NULL) free(response.mtext);
		//if(query.mtext!=NULL) free(query.mtext);
	}
	return 0;
}
