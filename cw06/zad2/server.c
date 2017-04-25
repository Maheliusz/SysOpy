#include"header.h"

mqd_t serverq;
mqd_t client;

void finish(int signo){
	mq_close(serverq);
	mq_close(client);
	while(mq_unlink(PROJECTQ)==-1);
	exit(0);
}

void server_stop(){
	struct mq_attr attr;
	mq_getattr(serverq, &attr);
	if(attr.mq_curmsgs==0){
		mq_close(serverq);
		while(mq_unlink(PROJECTQ)==-1);
		exit(0);
	}
}

int main(int argc, char *argv[]){
	struct mq_attr attr;
	attr.mq_flags=O_NONBLOCK;
	attr.mq_maxmsg = 20;
	attr.mq_msgsize = MSGSIZE;
	attr.mq_curmsgs=0;
	if((serverq = mq_open(PROJECTQ, O_RDWR | O_CREAT | O_EXCL | O_NONBLOCK, 
	0777, &attr))==-1){
		printf("Cannot create queue\n");
		exit(1);
	}
	time_t t;
	struct tm tmm;
	char line[128];
	char buf[128];
	char *addr;
	int stop=0;
	signal(SIGINT, finish);
	while(1){
		while(mq_receive(serverq, line, 128, NULL)<=0);
		buf = strtok(line, " \n\0");
		printf("Received message\n");
		if(strcmp(buf, "echo")){
			buf = strtok(NULL, " \n\0");
			addr = realloc(addr, (strlen(buf)+1)*sizeof(char));
			strcpy(addr, buf);
			buf = strtok(NULL, "\n\0");
			client = mq_open(addr, O_RDWR);
			mq_send(client, buf, 128, 1);
			mq_close(client);
		}else if(strcmp(buf, "wers")){
			buf = strtok(NULL, " \n\0");
			addr = realloc(addr, (strlen(buf)+1)*sizeof(char));
			strcpy(addr, buf);
			buf = strtok(NULL, "\n\0");
			for(int i=0; i<128; i++){
				if(buf[i]>=97&&buf[i]<=122) buf[i]=buf[i]-32;
			}
			client = mq_open(addr, O_RDWR);
			mq_send(client, buf, 128, 1);
			mq_close(client);
		}else if(strcmp(buf, "time")){
			buf = strtok(NULL, " \n\0");
			addr = realloc(addr, (strlen(buf)+1)*sizeof(char));
			strcpy(addr, buf);
			t = time(NULL);
			tmm = *localtime(&t);
			//usleep(100);
			//printf("Sending time\n");
			strftime(buf, 128, "%c", tmm);
			client = mq_open(addr, O_RDWR);
			mq_send(client, buf, 128, 1);
			mq_close(client);
		}else if(strcmp(buf, "stop")){
			stop=1;
		}
		printf("Processed message\n");
		if(stop) server_stop();
	}
}
