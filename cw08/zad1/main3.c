#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/syscall.h>

int threadnum;
char *filename;
int recordcount;
char *targetword;
FILE* file;
pthread_t *threadid;
pthread_attr_t attr;
int ongoing;


void sighandler(int signo){
	fclose(file);
	pthread_exit(NULL);
}

void* threadfun(void* args){
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	char buf[1024];
	char num[8];
	//int found=0;
	//while(1){
		for(int i=0; i<recordcount; i++){
			if(fread(buf, sizeof(char), 1024, file)<=0){
				ongoing--;
				sighandler(0);
			}
			strncpy(num, buf, 8);
			if(strstr(buf, targetword)!=NULL){
				printf("Word %s found by thread %ld in %s record\n", targetword, (long)(pthread_self()), num);
				//found=1;
			}
		}
		//pthread_testcancel();
		/*
		if(found){
			for(int i=0; i<threadnum; i++){
				if(threadid[i]!=0 && threadid[i]!=pthread_self())
					pthread_cancel(threadid[i]);
			}
		}
		*/
	//}
	ongoing--;
	return NULL;
}

int main(int argc, char* argv[]){
	if(argc<5) return 1;
	threadnum = atoi(argv[1]);
	filename=calloc(strlen(argv[2])+1, sizeof(char));
	ongoing=threadnum;
	strcpy(filename, argv[2]);
	recordcount = atoi(argv[3]);
	targetword=calloc(strlen(argv[4])+1, sizeof(char));
	strcpy(targetword, argv[4]);
	file = fopen(filename, "r");
	threadid=calloc(threadnum, sizeof(pthread_t));
	signal(SIGINT, sighandler);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	for(int i=0; i<threadnum; i++){
		if(pthread_create(&threadid[i], &attr, threadfun, NULL)==-1){
			printf("Cannot create thread\n");
			return 1;
		}
	}
	
	for(int i=0; i<threadnum; i++){
		pthread_join(threadid[i], NULL);
	}
	while(ongoing>0);
	fclose(file);
	return 0;
}
