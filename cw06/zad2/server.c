#include"header.h"

mqd_t serverq;

void finish(int signo){
	mq_close(serverq);
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
	char line[128];
	char buf[128];
	char addr[128];
	int stop=0;
	signal(SIGINT, finish);
	while(1){
		while(mq_receive(serverq, line, 128, NULL)<=0);
		buf = strtok(line, " \n\0");
		printf("Received message\n");
		if(strcmp(buf, "echo")){
			buf = strtok(NULL, " \n\0");
			strcpy(addr, buf);
			buf = strtok(NULL, "\n\0");
		}else if(strcmp(buf, "wers")){
			buf = strtok(NULL, " \n\0");
			strcpy(addr, buf);
			buf = strtok(NULL, "\n\0");
			for(int i=0; i<128; i++){
				if(buf[i]>=97&&buf[i]<=122) buf[i]=buf[i]-32;
			}
		}else if(strcmp(buf, "time")){
			buf = strtok(NULL, " \n\0");
			strcpy(addr, buf);
		}else if(strcmp(buf, "stop")){
			stop=1;
		}
		printf("Processed message\n");
		if(stop) server_stop();
	}
}
