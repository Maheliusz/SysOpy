#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>
#include<sys/types.h>
#include<time.h>

int n;
int m;
int *childrenpid;
int *requests;
int req_rnr;
clock_t send_time;
union sigval value;

void sighandler(int signo, siginfo_t *info, void *context){
	if(signo==SIGUSR1){
		printf("Parent: odebrano SIGUSR1; ");
		if(req_rnr<m){
			printf("zatrzymuje do otrzymania M requestow\n");
			requests[req_rnr]=info->si_uid;
			req_rnr++;
			if(req_rnr==m){
				printf("Otrzymano M requestow, przepuszczam wszystkie zatrzymane\n");
				for(int i=0; i<m; i++) kill(requests[i], SIGCONT);
			}
		}else{
			printf("przepuszczam proces\n");
			kill(info->si_uid, SIGCONT);
		}
	}
	else if(signo==SIGCONT){
		clock_t cont_time = clock();
		printf("Child: odebrano SIGCONT, koncze dzialanie\n");
		int res = (int)difftime(cont_time, send_time);
		exit(res);
	}
	else if(signo==SIGINT){
		for(int i=0; i<n; i++){
			if(childrenpid[i]!=0) kill(childrenpid[i], SIGKILL);
		}
		exit(0);
	}
}

int main(int argc, char *argv[]){
	if(argc<3) return 0;
	struct sigaction act;
	act.sa_handler = sighandler;
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
	srand((int)ppid);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
	for(int i=0; i<n; i++){
		pid = fork();
		childrenpid[i] = pid;
		if(pid==0) break;
		pause();
		printf("Parent: odebrano sygnal\n");	
	}
	if(pid==0){
		sigaction(SIGCONT, &act, NULL);
		sleep((rand()%10)+1);
		send_time = clock();
		kill(ppid, SIGUSR1);
		printf("Child: wyslano SIGUSR1\n");
		pause();
	}	
	return 0;
}
