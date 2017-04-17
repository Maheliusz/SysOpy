#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(int argc, char *argv[]){
	char ***container;
	container=calloc(1, sizeof(char**));
	container[0]=calloc(1, sizeof(char*));
	char *input = calloc(1000, sizeof(char));
	char *buf = calloc(100, sizeof(char));
	int run1 = 0;
	int run2 = 0;
	/*for(int i=0; i<100; i++){
		for(int j=0; j<100; j++)
			container[i][j]=NULL;
	}*/
	fgets(input, 1000, stdin);
	if((buf = strtok(input, " \n\0"))==NULL){ 
		printf("BLAD");
		exit(1);
	}
	do{
		//if(buf == NULL) container[run1]=NULL;
		if(strcmp(buf, "|") == 0){
			run1++;
			container = realloc(container, (run1+1)*sizeof(char**));
			container[run1]=calloc(1, sizeof(char*));
			run2 = 0;
		}
		else{
			container[run1]= realloc(container, (run2+1)*sizeof(char*));
			container[run1][run2]=calloc(strlen(buf)+1, sizeof(char));
			strcpy(container[run1][run2], buf);
			run2++;
		}
	}while((buf = strtok(NULL, " \n\0"))!=NULL);
	//container[run1+1][0] = NULL;
	free(input);
	free(buf);
	/*for(int i=0; i<100; i++){
		for(int j=0; j<100; j++) printf("%s", container[i][j]);
		printf("\n");
	}*/
	
	run1 = 0;
	int pr[2];
	int pw[2];
	pid_t pid;
	int length = sizeof(container)/sizeof(container[0]);
	for(int i=0; i<length; i++){
		if(container[i][0]==NULL) break;
		pipe(pr);
		if(i==0) pipe(pw);
		else{
			int tmp0=pw[0];
			int tmp1=pw[1];
			pw[0]=pr[0];
			pw[1]=pr[1];
			pr[0]=tmp0;
			pr[1]=tmp1;
		}		
		pid = fork();
		if(pid==0){
			if(i>0){
				close(pr[1]);
				dup2(pr[0], STDIN_FILENO);
			}
			if(i<length-1){
				close(pw[0]);
				dup2(pw[1], STDOUT_FILENO);
			}
			execvp(container[i][0], container[i]);
		}
		close(pr[0]);
		close(pr[1]);
	}
	close(pw[0]);
	close(pw[1]);
	close(pr[0]);
	close(pr[1]);
	
	return 0;
}
