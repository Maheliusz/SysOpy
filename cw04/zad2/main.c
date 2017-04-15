#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>
#include<sys/types.h>

int n;
int m;
int *childrenpid;
int *requests;
int req_rnr;

void sighandler(int signo, siginfo_t *info){
	if(signo==SIGUSR1){
		if(req_rnr<=m){
			requests[req_rnr]=info->si_pid;
			req_rnr++;
		}else{
			kill(info->si_pid, SIGCONT);
		}
	}
	if(signo==SIGINT){
		for(int i=0; i<n; i++){
			if(childrenpid[i]!=0) kill(childrenpid[i], SIGKILL);
		}
		exit(0);
	}
}

int main(int argc, char *argv[]){
	if(argc<3) return 0;
	srand(time(NULL));
	struct sigaction act;
	act.sa_handler=sighandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	req_rnr=0;
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	if(m>n) return 1;
	childrenpid = calloc(n, sizeof(int));
	requests = calloc(n, sizeof(int));
	pid_t ppid = getpid();
	pid_t pid;
	for(int i=0; i<n; i++){
		sigaction(SIGINT, act, NULL);
		pid = fork();
		childrenpid[i] = pid;
		if(pid==0) break;	
	}
	if(pid==0){
		sleep(rand()%10);
		kill(ppid, SIGUSR1);
		pause();
		sigaction(SIGCONT, act, NULL);
	}
	else{
		for(int i=0; i<m; i++){
			pause();
			sigaction(SIGINT, act, NULL);
			sigaction(SIGUSR1, act, NULL;
		}
	}
	
	return 0;
}
