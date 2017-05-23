#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>

int main(int argc, char *argv[]){
	if(argc<3) return 1;
	int size = atoi(argv[1]);
	char *name = calloc(strlen(argv[2])+1, sizeof(char));
	char str[1015];
	strcpy(name, argv[2]);
	FILE* file = fopen(name, "w");
	for(int i=0; i<size; i++){
		for(int j=0; j<1014; j++){
			str[j]=rand()%26+'a';
		}
		fprintf(file, "%08d-%s\n", i, str);
	}
	fclose(file);
	chmod(name, 0777);
	return 0;
}
