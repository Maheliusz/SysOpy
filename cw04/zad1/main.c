#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int fwd=1;

void sighandler(int signo){
	if(signo==SIGTSTP){
		if(fwd==1) fwd=-1;
		else fwd = 1;
	}
	else if(signo==SIGINT){
		printf("\nOdebrano sygnal SIGINT\n");
		exit(0);
	}
}

int main(int argc, char* argv[]){
	const char *alphabet = "abcdefghijklmonpqrstuvwxyz";
	int rnr = 0;
	struct sigaction action;
	action.sa_handler = sighandler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	while(1){
		signal(SIGTSTP, sighandler);
		sigaction(SIGINT, &action, NULL);
		printf("%c", alphabet[rnr]);
		rnr+=fwd;
		if(rnr==strlen(alphabet)) rnr=0;
		else if(rnr==-1) rnr=strlen(alphabet)-1;
		usleep(100);
	}
	return 0;
}
