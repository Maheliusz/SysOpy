#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void filltab(int **T, int R, double i, double j, int iters){
	int re_res = 0;
	int im_res = 0;
	for(int a = 0; a<R; a++){
		if((-2+a*3/R)<=i&&(-2+(a+1)*3/R)>=i) re_res = a;
		if((-1+a*3/R)<=j&&(-1+(a+1)*3/R)>=j) im_res= a;
	}
	T[re_res][im_res]=iters;
}

int main(int argc, char* argv[]){
	if(argc<3) return 1;
	int R = atoi(argv[2]);
	int **T = calloc(R, sizeof(int*));
	for(int i=0; i<R; i++) T[i] = calloc(R, sizeof(int));
	char *path = calloc(strlen(argv[1]), sizeof(char));
	strcpy(path, argv[1]);
	FILE *fifo = fopen(path, "r");
	if(fifo==NULL) exit(1);
	//printf("Otwarlem plik\n");
	char *line = calloc(128, sizeof(char));
	char *buf = calloc(128, sizeof(char));
	double i, j;
	int iters;
	//int tmp=0;
	while(fgets(line, 128, fifo)!=0){
		strcpy(buf, line);
		sscanf(strtok(buf, " \n"), "%lf", &i);
		sscanf(strtok(NULL, " \n"), "%lf", &j);
		iters = atoi(strtok(NULL, " \n"));
		filltab(T, R, i, j, iters);
		//printf("Przeczytalem linijke numer %d\n", tmp++);
	}
	fclose(fifo);
	//printf("Tablica zapelniona\n");
	FILE *data = fopen("data", "w+");
	chmod("data", S_IRWXU | S_IRWXG | S_IRWXO);
	for(int a=0; a<R; a++){
		for(int b=0; b<R; b++){
			fprintf(data, "%d %d %d\n", a, b, T[a][b]);
		}
	}
	//printf("Utworzono data\n");
	
	FILE *plot = popen("gnuplot -persistent", "w");
	printf("Uruchomilem gnuplot\n");
	fprintf(plot, "set view map");
	fprintf(plot, "set xrange [0:%d]", R);
	fprintf(plot, "set yrange [0:%d]", R);
	fprintf(plot, "plot 'data' with image");
	fflush(plot);
	getc(stdin);
	fclose(data);
	pclose(plot);
	return 0;
}
