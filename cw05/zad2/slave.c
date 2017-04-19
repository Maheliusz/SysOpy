#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

int K;

int iters(double re, double im){
	double curr_re = 0;
	double curr_im = 0;
	int i=0;
	while(1){
		if(i>=K) break;
		curr_re = curr_re*curr_re - curr_im*curr_im + re;
		curr_im = 2*curr_im*curr_re + im;
		i++;
		if(sqrt(curr_re*curr_re+curr_im*curr_im)>=2) break;
	}
	return i;
}

double rand_re(){
	//return 3*rand()/RAND_MAX - 1;
	return (3*(double)rand()/(double)(RAND_MAX)-2);
}

double rand_im(){
	//return 2*rand()/RAND_MAX - 1;
	return (2*(double)rand()/(double)(RAND_MAX)-1);
}

int main(int argc, char* argv[]){
	if(argc<4) return 1;
	char *path = calloc(strlen(argv[1]), sizeof(char));
	strcpy(path, argv[1]);
	mkfifo(path, S_IRWXU);
	int N = atoi(argv[2]);
	K = atoi(argv[3]);
	FILE *file = fopen(path, "w");
	srand(time(NULL));
	double re;
	double im;
	for(int i=0; i<N; i++){
		re = rand_re();
		im = rand_im();
		fprintf(file, "%lf %lf %d\n", re, im, iters(re, im));
		//printf("%lf %lf %d\n", re, im, iters(re, im));
	}
	fclose(file);	
	return 0;
}
