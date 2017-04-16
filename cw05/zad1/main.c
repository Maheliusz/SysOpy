#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(int argc, char *argv[]){
	char ***container;
	container=calloc(100, sizeof(char**));
	for(int i=0; i<100; i++) container[i]=calloc(100, sizeof(char*));
	char *input = calloc(1000, sizeof(char));
	char *buf = calloc(100, sizeof(char));
	int run1 = 0;
	int run2 = 0;
	fgets(input, 100, stdin);
	if((buf = strtok(input, " \n"))==NULL){ 
		printf("BLAD");
		exit(1);
	}
	do{
		//if(buf == NULL) container[run1]=NULL;
		if(strcmp(buf, "|") == 0){
			run1++;
			run2 = 0;
		}
		else{
			strcpy(container[run1][run2], buf);
		}
		run2++;
	}while((buf = strtok(NULL, " \n"))!=NULL);
	container[run1+1][0] = NULL;
	free(input);
	free(buf);
	run1 = 0;
	//int argcnt = sizeof(container);
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	while(container[run1][0]!=NULL){
		if(pid==0){
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			execvp(container[run1][0], container[run1]+1);
		}
		else{
			close(fd[0]);
			write(fd[1], STDOUT_FILENO);
		}
		run1++;
		pid = fork();
	}
	
	return 0;
}
