#include "header.h"

int queue;
int stop;

void server_stop(){
	struct msqid_ds stat;
	msgctl(queue, IPC_STAT, &stat);
	if(stat.msg_qnum==0){ 
		msgctl(queue, IPC_RMID, NULL);
		printf("Server stop\n");
		exit(0);
	}
}

void finish(int signo){
	msgctl(queue, IPC_RMID, NULL);
	printf("Received interruption signal\n");
	exit(0);
}

int main(int argc, char *argv[]){
	stop=0;
	queue = msgget(ftok(getenv("HOME"), PROJECTID), IPC_CREAT | 0777);
	if(queue==-1){
		printf("Can't create queue\n");
		exit(1);
	} 
	time_t t;
	struct tm *tmm = {0};
	signal(SIGINT, finish);
	struct clientbuf query;
	//struct clientbuf response;
	query.mtype = ANSWER;
	while(1){
		while(msgrcv(queue, &query, MSGSIZE, 0, 0)==-1);
		printf("Received message\n");
		if(query.mtype==ECHO){
			//printf("Arg:%s\n", query.mtext);
			//usleep(100);
			//printf("Echo\n");
			msgsnd(query.qid, &query, MSGSIZE, 0);
		}else if(query.mtype==WERS){
			//printf("Arg:%s\n", query.mtext);
			for(int i=0; i<strlen(query.mtext); i++){
				if((int)query.mtext[i]>=97&&(int)query.mtext[i]<=122){
					query.mtext[i]=query.mtext[i]-32;
				}
				else query.mtext[i]=query.mtext[i];
			}
			//printf("Uppercase\n");
			//usleep(100);
			msgsnd(query.qid, &query, MSGSIZE, 0);
		}else if(query.mtype==TIME){
			t = time(NULL);
			tmm = localtime(&t);
			//usleep(100);
			//printf("Sending time\n");
			if(tmm!=NULL) strftime(query.mtext, 128, "%c", tmm);
			else strcpy(query.mtext, "0");
			msgsnd(query.qid, &query, MSGSIZE, 0);
		}else if(query.mtype==STOP){
			//usleep(100);
			stop=1;
			//printf("Stopping server\n");
			//exit(0);
		}
		printf("Processed message\n");
		if(stop) server_stop();
	}
	return 0;
}
