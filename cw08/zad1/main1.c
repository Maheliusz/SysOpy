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
//int currentrec;
FILE* file;
pthread_t *threadid;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INTIALIZER;

/*
int containsword(char* str, char* word){
	if(strlen(str)<strlen(word)) return 0;
	for(int i=0; i<strlen(str)-strlen(word); i++){
		if(strcmp(str+i, word)==0) return 1;
	}
	return 0;
}
*/

void sighandler(int signo){
	fclose(file);
	pthread_exit((void*)signo);
}

void* threadfun(void* args){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	char buf[1024];
	char num[8];
	//char *num=calloc(8, sizeof(char));
	char str[1024];
	int found=0;
	for(int i=0; i<recordcount; i++){
		fread(buf, sizeof(char), 1024, file);
		strncpy(num, buf, 8);
		strncpy(str, buf, 1024);
		//num=strtok(buf, "-");
		if(strstr(str, targetword)!=NULL){
			printf("Word %s found by thread %ld in %s record\n", targetword, (long)(pthread_self()), num);
			found=1;
			break;
		}
	}
	if(found){
		for(int i=0; i<threadnum; i++){
			if(threadid[i]!=0 && threadid[i]!=pthread_self())
				pthread_cancel(threadid[i]);
		}
	}
	return NULL;
}

int main(int argc, char* argv[]){
	if(argc<5) return 1;
	threadnum = atoi(argv[1]);
	filename=calloc(strlen(argv[2])+1, sizeof(char));
	strcpy(filename, argv[2]);
	recordcount = atoi(argv[3]);
	targetword=calloc(strlen(argv[4])+1, sizeof(char));
	strcpy(targetword, argv[4]);
	//currentrec=0;
	file = fopen(filename, "r");
	threadid=calloc(threadnum, sizeof(pthread_t));
	signal(SIGINT, sighandler);
	for(int i=0; i<threadnum; i++){
		if(pthread_create(&threadid[i], NULL, threadfun, NULL)==-1){
			printf("Cannot create thread\n");
			return 1;
		}
	}
	for(int i=0; i<threadnum; i++){
		pthread_join(threadid[i], NULL);
	}
	fclose(file);
	return 0;
}
