#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

int K;

int iters(int re, int im){
	int curr_re = 0;
	int curr_im = 0;
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

int rand_re(){
	return (rand()%4)-2;
}

int rand_im(){
	return (rand()%3)-2;
}

int main(int argc, char* argv[]){
	if(argc<4) return 1;
	char *path = calloc(strlen(argv[1]), sizeof(char));
	strcpy(path, argv[1]);
	int N;
	N = atoi(argv[2]);
	K = atoi(argv[3]);
	FILE *file = fopen(path, "w");
	srand(time(NULL));
	int re;
	int im;
	int res;
	for(int i=0; i<N; i++){
		re = rand_re();
		im = rand_im();
		res = iters(re, im);
		fprintf(file, "%d %d %d\n", re, im, res);
	}
	fclose(file);	
	return 0;
}
