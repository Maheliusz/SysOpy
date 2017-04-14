#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>

int pid;
int ppid;

void parentrthandler(int signo){
	if(signo==SIGRTMIN){
		printf("Parent: otrzymano SIGRTMIN\n");
	}else if(signo==SIGRTMAX){
		printf("Parent: otrzymano SIGRTMAX, koncze dzialanie\n");
		exit(0);
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
		for(int i=0; i<L; i++){
			pause();
			signal(SIGUSR1, childkillhandler);
		}
		kill(ppid, SIGUSR2);
		exit(0);
	}else{
		for(int i=0; i<L; i++){
			signal(SIGINT, parentkillhandler);
			kill(pid, SIGUSR1);
			printf("Parent: wyslano SIGUSR1 nr %d\n", i);
			pause();
			signal(SIGUSR1, parentkillhandler);
			signal(SIGUSR2, parentkillhandler);
		}
	}
}

void sigq(int L){
	union sigval value;
	ppid = getpid();
	pid = fork();
	if(pid==0){
		for(int i=0; i<L; i++){
			pause();
			signal(SIGUSR1, childqhandler);
		}
		sigqueue(ppid, SIGUSR2, value);
		exit(0);
	}else{
		for(int i=0; i<L; i++){
			signal(SIGINT, parentqhandler);
			sigqueue(pid, SIGUSR1, value);
			printf("Parent: wyslano SIGUSR1 nr %d\n", i);
			pause();
			signal(SIGUSR1, parentqhandler);
			signal(SIGUSR2, parentqhandler);
		}
	}
}

void rtsig(int L){
	ppid = getpid();
	pid = fork();
	if(pid==0){
		for(int i=0; i<L; i++){
			pause();
			signal(SIGRTMIN, childrthandler);
		}
		kill(ppid, SIGRTMAX);
		exit(0);
	}else{
		for(int i=0; i<L; i++){
			signal(SIGINT, parentrthandler);
			kill(pid, SIGRTMIN);
			printf("Parent: wyslano SIGRTMIN nr %d\n", i);
			pause();
			signal(SIGRTMIN, parentrthandler);
			signal(SIGRTMAX, parentrthandler);
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
