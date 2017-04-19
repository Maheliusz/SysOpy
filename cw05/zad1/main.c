#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

void execute(int input, int output, char **command){
	pid_t pid = fork();
	if(pid==0){
		if(input!=STDIN_FILENO){
			dup2(input, STDIN_FILENO);
			close(input);
		}
		if(output!=STDOUT_FILENO){
			dup2(output, STDOUT_FILENO);
			close(output);
		}
		execvp(command[0], command);
	}
	else{
		wait(NULL);
	}
}

int main(int argc, char *argv[]){
	char ***container;
	container=calloc(1, sizeof(char**));
	container[0]=calloc(1, sizeof(char*));
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
			container = realloc(container, (run1+1)*sizeof(char**));
			container[run1]=calloc(1, sizeof(char**));
			run2 = 0;
		}
		else{
			container[run1]= realloc(container[run1], (run2+1)*sizeof(char*));
			container[run1][run2]=calloc(strlen(buf)+1, sizeof(char));
			strcpy(container[run1][run2], buf);
			run2++;
		}
	}while((buf = strtok(NULL, " \n\0"))!=NULL);
	free(input);
	free(buf);
	int pp[2];
	//pid_t pid;
	int length = run1+1;
	int in = STDIN_FILENO;
	int out = pp[0];
	for(int i=0; i<length; i++){
		pipe(pp);
		if(i==length-1) out = STDOUT_FILENO;
		execute(in, out, container[i]);
		close(pp[0]);
		in = pp[1];
		/*
		pid = fork();
		//close(pp[0]);
		//close(pp[1]);
		if(pid==0){
			if(i!=0){
				dup2(pp[0], STDIN_FILENO);
				close(pp[0]);
			}
			if(i!=length-1){
				dup2(pp[1], STDOUT_FILENO);
				close(pp[1]);
			}
			execvp(container[i][0], container[i]);
			close(pp[1]);
			//printf("Wykonano komende nr %d\n", i);
		}
		else{
			wait(NULL);
		}
		*/
	}	
	return 0;
}
