#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/syscall.h>

static const int SIGNAL = SIGINT;
int threadnum;
int version;
pthread_t *threadid;


void sighandler(int signo){
	printf("Pid: %d, Tid: %ld, signo: %d\n", getpid(), pthread_self(), signo);
	//pthread_exit(NULL);
}

void* threadfun(void* args){
	signal(SIGFPE, sighandler);
	pthread_sigmask(SIG_UNBLOCK, NULL, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int s;
	if(args!=NULL) s = 1/0;
	if(version==3) signal(SIGNAL, sighandler);
	else if(version==4){
		sigset_t set;
		sigfillset(&set);
		pthread_sigmask(SIG_BLOCK, &set, NULL);
		
	}
	else if(version==5)signal(SIGNAL, sighandler);
	while(1);
	return NULL;
}

int main(int argc, char* argv[]){
	if(argc<3) return 1;
	threadnum = atoi(argv[1]);
	version=atoi(argv[2]);
	sigset_t set;
	sigemptyset(&set);
	threadid=calloc(threadnum, sizeof(pthread_t));
	signal(SIGNAL, sighandler);
	for(int i=0; i<threadnum; i++){
		if(i==threadnum-1) pthread_create(&threadid[i], NULL, threadfun, (void*)0);
		else if(pthread_create(&threadid[i], NULL, threadfun, NULL)==-1){
			printf("Cannot create thread\n");
			return 1;
		}
	}
	switch(version){
		case 1:
			raise(SIGNAL);
			break;
		case 2:
			sigfillset(&set);
			sigprocmask(SIG_BLOCK, &set, NULL);
			raise(SIGNAL);
			break;
		case 3:
			raise(SIGNAL);
			break;
		case 4:
			for(int i=0; i<threadnum; i++){
				pthread_kill(threadid[i], SIGNAL);
			}
			break;
		case 5:
			for(int i=0; i<threadnum; i++){
				pthread_kill(threadid[i], SIGNAL);
			}
			break;
		default:
			break;
	}
	//for(int i=0; i<threadnum; i++){
	//	pthread_join(threadid[i], NULL);
	//}
	return 0;
}
