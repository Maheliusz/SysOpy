#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int **T;
int R;

void filltab(double i, double j, int iters){
	int re_res=0;
	int im_res=0;
	for(re_res=1; 3.0*re_res/((double)R)-2<i; re_res++);
	for(im_res=1; 2.0*im_res/((double)R)-1<j; im_res++);
	/*for(int a = 0; a<R; a++){
		if((-2+a*3/R)<=i&&(-2+(a+1)*3/R)>=i) re_res = a;
		if((-1+a*2/R)<=j&&(-1+(a+1)*3/R)>=j) im_res= a;
	}*/
	T[re_res-1][im_res-1]=iters;
}

int main(int argc, char* argv[]){
	if(argc<3) return 1;
	R = atoi(argv[2]);
	T = calloc(R, sizeof(int*));
	for(int i=0; i<R; i++) T[i] = calloc(R, sizeof(int));
	for(int i=0; i<R; i++){
		for(int j=0; j<R; j++) T[i][j]=0;
	}
	char *path = calloc(strlen(argv[1]), sizeof(char));
	strcpy(path, argv[1]);
	mkfifo(path, S_IRWXU);
	FILE *fifo = fopen(path, "r");
	if(fifo==NULL) exit(1);
	//printf("Otwarlem plik\n");
	double i, j;
	int iters;
	//int tmp=0;
	while(fscanf(fifo, "%lf %lf %d", &i, &j, &iters)!=EOF){
		//printf("Przeczytalem linijke numer %d\n", tmp++);
		//printf("%lf %lf %d\n", i, j, iters);
		filltab(i, j, iters);
	}
	fclose(fifo);
	//printf("Tablica zapelniona\n");
	FILE *data = fopen("data", "w");
	chmod("data", S_IRWXU | S_IRWXG | S_IRWXO);
	for(int a=0; a<R; a++){
		for(int b=0; b<R; b++){
			fprintf(data, "%d %d %d\n", a, b, T[a][b]);
		}
	}
	//printf("Utworzono data\n");
	fclose(data);
	FILE *plot = popen("gnuplot -persistent", "w");
	//printf("Uruchomilem gnuplot\n");
	fprintf(plot, "set view map\n");
	fprintf(plot, "set xrange [0:%d]\n", R);
	fprintf(plot, "set yrange [0:%d]\n", R);
	fprintf(plot, "plot 'data' with image\n");
	fflush(plot);
	getc(stdin);
	pclose(plot);
	return 0;
}
