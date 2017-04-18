#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){
	if(argc<3) return 1;
	int R = atoi(argv[2]);
	int **T = calloc(R, sizeof(int*));
	for(int i=0; i<R; i++) T[i] = calloc(R, sizeof(int));
	char *path = calloc(strlen(argv[1]), sizeof(char));
	strcpy(path, argv[1]);
	if(mkfifo(path, S_IRWXU)!=0) exit(1);
	FILE *fifo = fopen(path, "r");
	char *line = calloc(128, sizeof(char));
	char *buf = calloc(128, sizeof(char));
	int i, j;
	while(fread(line, sizeof(char), 128, fifo)!=0){
		buf = strtok(line, " \n\0");
		i = atoi(buf);
		free(buf);
		buf = strtok(NULL, " \n\0");
		j = atoi(buf);
		free(buf);
		buf = strtok(NULL, " \n\0");
		T[i][j]+= atoi(buf);
		free(buf);
	}
	fclose(fifo);
	FILE *plot = popen("gnuplot", "w");
	fprintf(plot, "set view map");
	fprintf(plot, "set xrange [0:%d]", R);
	fprintf(plot, "set yrange [0:%d]", R);
	fprintf(plot, "plot 'data' with image");
	fflush(plot);
	getc(stdin);
	return 0;
}
