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
	double module;
	while(1){
		if(i==K) break;
		curr_re = curr_re*curr_re - curr_im*curr_im + re;
		curr_im = 2*curr_im*curr_re + im;
		i++;
		module = sqrt(curr_re*curr_re+curr_im*curr_im);
		if(module>=2) break;
	}
	return i;
}

double rand_re(){
	return ((double)rand()/(double)(RAND_MAX/3)-2);
}

double rand_im(){
	return ((double)rand()/(double)(RAND_MAX/2)-1);
}

int main(int argc, char* argv[]){
	if(argc<4) return 1;
	char *path = calloc(strlen(argv[1]), sizeof(char));
	strcpy(path, argv[1]);
	mkfifo(path, S_IRWXU);
	int N;
	N = atoi(argv[2]);
	K = atoi(argv[3]);
	FILE *file = fopen(path, "w");
	srand(time(NULL));
	double re;
	double im;
	int res;
	for(int i=0; i<N; i++){
		re = rand_re();
		im = rand_im();
		res = iters(re, im);
		fprintf(file, "%lf %lf %d\n", re, im, res);
	}
	fclose(file);	
	return 0;
}
