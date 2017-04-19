#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char *argv[]){
	char ***container=calloc(1, sizeof(char**));
	container[0]=calloc(1, sizeof(char*));
	char *input = calloc(1000, sizeof(char)); 
	char *buf = calloc(100, sizeof(char));

	int run1;
	int run2;
  //while(1){
	run1 = 0;
	run2 = 0;
	//container=calloc(1, sizeof(char**));
	//container[0]=calloc(1, sizeof(char*));
	//input = calloc(1000, sizeof(char));
	//buf = calloc(100, sizeof(char));

	fgets(input, 1000, stdin);
	//if(strcmp(input, "quit")==0 || strcmp(input, "exit")==0) break;
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
	pid_t pid;
	int length = run1+1;
	int *pp = calloc(2*(length+1), sizeof(int));
	for(int i=0; i<length+2; i++) pipe(&pp[2*i]);
	for(int i=0; i<length; i++){
		pid = fork();
		if(pid==0){
			if(i!=0){
				dup2(pp[2*i-2], 0);
			}
			if(i!=length-1){
				dup2(pp[2*i+1], 1);
			}
			close(pp[2*i+1]);
			execvp(container[i][0], container[i]);
		}
		close(pp[2*i+1]);
		wait(NULL);
	}
	//for(int i=0; i<length; i++) free(container[i]);
	//}	
	return 0;
}
