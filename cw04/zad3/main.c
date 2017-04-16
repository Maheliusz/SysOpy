#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>

int pid;
int ppid;
char *msg;

void parentrthandler(int signo){
	if(signo==SIGRTMIN){
		printf("Parent: otrzymano SIGRTMIN\n");
	}else if(signo==SIGRTMAX){
		printf("Parent: otrzymano SIGRTMAX, koncze dzialanie\n");
	} else if(signo==SIGINT){
		kill(pid, SIGINT);
	}
}

void childrthandler(int signo){
	if(signo==SIGRTMIN){
		printf("Child: otrzymano SIGRTMIN, odsylam\n");
		kill(ppid, SIGRTMIN);
	}
}

void parentqhandler(int signo){
	switch(signo){
		case SIGUSR1:
			printf("Parent: otrzymano SIGUSR1\n");
			break;
		case SIGUSR2:
			printf("Parent: otrzymano SIGUSR2, koncze dzialanie\n");
			exit(0);
			break;
		case SIGINT:
			kill(pid, SIGINT);
			break;
	}
	fflush(NULL);
}

void childqhandler(int signo){
	union sigval value;
	switch(signo){
		case SIGUSR1:
			printf("Child: otrzymano SIGUSR1, odsylam\n");
			sigqueue(ppid, SIGUSR1, value);
			break;
	}
}

void parentkillhandler(int signo){
	switch(signo){
		case SIGUSR1:
			printf("Parent: otrzymano SIGUSR1\n");
			break;
		case SIGUSR2:
			printf("Parent: otrzymano SIGUSR2, koncze dzialanie\n");
			exit(0);
			break;
		case SIGINT:
			kill(pid, SIGINT);
			break;
	}
}

void childkillhandler(int signo){
	switch(signo){
		case SIGUSR1:
			printf("Child: otrzymano SIGUSR1, odsylam\n");
			kill(ppid, SIGUSR1);
			break;
	}
}

void killfun(int L){
	ppid = getpid();
	pid = fork();
	if(pid==0){
		signal(SIGUSR1, childkillhandler);
		signal(SIGINT, SIG_DFL);
		for(int i=0; i<L; i++){
			pause();
		}
		printf("Child: wysylam SIGUSR2\n");
		kill(ppid, SIGUSR2);
		exit(0);
	}else{
		signal(SIGINT, parentkillhandler);
		signal(SIGUSR1, parentkillhandler);
		signal(SIGUSR2, parentkillhandler);
		for(int i=0; i<L; i++){
			sleep(1);
			kill(pid, SIGUSR1);
			printf("Parent: wyslano SIGUSR1 nr %d\n", i+1);
			pause();
		}
	}
}

void sigq(int L){
	union sigval value;
	ppid = getpid();
	pid = fork();
	if(pid==0){
		signal(SIGUSR1, childqhandler);
		signal(SIGINT, SIG_DFL);
		for(int i=0; i<L; i++){
			pause();
		}
		printf("Child: wysylam SIGUSR2\n");
		sigqueue(ppid, SIGUSR2, value);
		exit(0);
	}else{
		signal(SIGINT, parentqhandler);
		signal(SIGUSR1, parentqhandler);
		signal(SIGUSR2, parentqhandler);
		for(int i=0; i<L; i++){
			sleep(1);
			sigqueue(pid, SIGUSR1, value);
			printf("Parent: wyslano SIGUSR1 nr %d\n", i+1);
			pause();
		}
	}
}

void rtsig(int L){
	ppid = getpid();
	pid = fork();
	if(pid==0){
		signal(SIGRTMIN, childrthandler);
		signal(SIGINT, SIG_DFL);
		for(int i=0; i<L; i++){
			pause();
		}
		printf("Child: wysylam SIGRTMAX\n");
		kill(ppid, SIGRTMAX);
		exit(0);
	}else{
		signal(SIGRTMIN, parentrthandler);
		signal(SIGRTMAX, parentrthandler);
		signal(SIGINT, parentrthandler);
		for(int i=0; i<L; i++){
			sleep(1);
			kill(pid, SIGRTMIN);
			printf("Parent: wyslano SIGRTMIN nr %d\n", i+1);
			pause();
		}
	}
}
int main(int argc, char* argv[]){
	if(argc<3) return 0;
	int L = atoi(argv[1]);
	int type = atoi(argv[2]);
	if(type<1||type>3){
		printf("Type moze przyjmowac wartosci wylacznie 1, 2, 3\n");
		return 1;
	}
	sigset_t *set = calloc(1, sizeof(sigset_t*));;
	sigemptyset(set);
	sigprocmask(SIG_SETMASK, set, NULL);
	switch(type){
		case 1:
			killfun(L);
			break;
		case 2:
			sigq(L);
			break;
		case 3:
			rtsig(L);
			break;	
		default:
			break;			
	}
	return 0;
}
