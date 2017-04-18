#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char *argv[]){
	char ***container;
	container=calloc(1, sizeof(char***));
	container[0]=calloc(1, sizeof(char**));
	char *input = calloc(1000, sizeof(char));
	char *buf = calloc(100, sizeof(char));
	int run1 = 0;
	int run2 = 0;
	
	fgets(input, 1000, stdin);
	if((buf = strtok(input, " \n\0"))==NULL){ 
		printf("BLAD");
		exit(1);
	}
	do{
		if(strcmp(buf, "|") == 0){
			run1++;
			container = realloc(container, (run1+1)*sizeof(char***));
			container[run1]=calloc(1, sizeof(char**));
			run2 = 0;
		}
		else{
			container[run1]= realloc(container, (run2+1)*sizeof(char**));
			container[run1][run2]=calloc(1, sizeof(char*));
			//calloc(strlen(buf)+1, sizeof(char));
			strcpy(container[run1][run2], buf);
			run2++;
		}
	}while((buf = strtok(NULL, " \n\0"))!=NULL);
	free(input);
	free(buf);
	int pp[2];
	pid_t pid;
	int length = sizeof(container)/sizeof(container[0]);
	int onlyOne=1;
	for(int i=0; i<length-1; i++){
		pipe(pp);
		pid = fork();
		if(pid==0){
			if(i!=0){
				dup2(pp[0], STDIN_FILENO);
				close(pp[0]);
			}
			dup2(pp[1], STDOUT_FILENO);
			close(pp[1]);
			execvp(container[i][0], container[i]);
		}
		else{
			int status;
			wait(&status);
		}
		onlyOne=0;
	}
	
	pid=fork();
	if(pid==0){
		if(onlyOne==0){
			dup2(pp[0], STDIN_FILENO);
			close(pp[0]);
		}
		execvp(container[length-1][0], container[length-1]);
	}
	else{
		int status;
		wait(&status);
	}
	
	return 0;
}
