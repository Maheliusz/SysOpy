#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>

int threadnum;
char *filename;
int recordcount;
char *targetword;
//int currentrec;
int file;
pthread_t *threadid;

int containsword(char* str, char* word){
	if(strlen(str)<strlen(word)) return 0;
	for(int i=0; i<strlen(str)-strlen(word); i++){
		if(strcmp(str+i, word)==0) return 1;
	}
	return 0;
}

void sighandler(int signo){
	close(file);
	pthread_exit(signo);
}

void* threadfun(void* args){
	char buf[1024];
	char num[8];
	char str[1014];
	for(int i=0; i<recordcount; i++){
		read(file, &buf, 1024);
		strncpy(num, buf, 8);
		strncpy(str, buf+9, 1014);
		if(containsword(str, targetword)){
			printf("Word %s found by thread %d in %s record\n", targetword, (int)(pthread_self()), num);
			break;
		}
	}
	for(int i=0; i<threadnum; i++){
		if(threadid[i]!=0 && threadid[i]!=pthread_self())
			pthread_kill(threadid[i], SIGINT);
	}
	pthread_exit(0);
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
	file = open(filename, O_RDONLY);
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
	close(file);
	return 0;
}
