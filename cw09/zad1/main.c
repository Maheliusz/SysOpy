#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<errno.h>

int descriptive;
int writernum;
int readernum;
int table[256];
sem_t* block;
pthread_t* writers;
pthread_t* readers;

void* writer(void* arg){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int cnt;
	int index;
	int value;
	while(1){
		sem_trywait(block);
		if(errno==EAGAIN) continue;
		cnt=rand()%32;
		for(int i=0; i<cnt; i++){
			index=rand()%256;
			value=rand()%256;
			table[index]=value;
			if(descriptive) printf("Pisarz %ld - index: %d, wartosc: %d\n", pthread_self(), index, value);
		}
		printf("Pisarz %ld: zmodyfikowano tablice\n", pthread_self());
		usleep(750000);
		sem_post(block);
	}
	return NULL;
}

void* reader(void* arg){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int cntr;
	int div= *((int *)arg);
	while(1){
		cntr=0;
		sem_wait(block);
		for(int i=0; i<256; i++){
			if(table[i]%div){
				cntr++;
				if(descriptive) printf("Czytelnik %ld: index: %d, wartosc: %d\n", pthread_self(), i, table[i]);
			}		
		}
		printf("Czytelnik %ld: znaleziono %d liczb podzielnych przez %d\n", pthread_self(), cntr, div);
		usleep(750000);
		sem_post(block);
	}
	return NULL;
}

void sighandler(int signo){
	for(int i=0; i<writernum; i++) pthread_cancel(writers[i]);
	for(int i=0; i<readernum; i++) pthread_cancel(readers[i]);
	sem_close(block);
	exit(signo);
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	if(argc<3) return 1;
	writernum = atoi(argv[1]);
	readernum = atoi(argv[2]);
	writers=calloc(writernum, sizeof(pthread_t));
	readers=calloc(readernum, sizeof(pthread_t));
	if(argc>3 && strcmp(argv[3], "-i")==0) descriptive=1;
	else descriptive=0;
	block=malloc(sizeof(sem_t));
	sem_init(block, 1, 1);
	for(int i=0; i<writernum; i++){
		pthread_create(&writers[i], NULL, writer, NULL);
	}
	int div;
	for(int i=0; i<readernum; i++){
		div = rand()%256;
		pthread_create(&readers[i], NULL, reader, (void*)&div);
	}
	signal(SIGINT, sighandler);
	for(int i=0; i<writernum; i++){
		pthread_join(writers[i], NULL);
	}
	for(int i=0; i<readernum; i++){
		pthread_join(readers[i], NULL);
	}
	return 0;
}
