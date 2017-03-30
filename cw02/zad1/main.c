#include <time.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int positionInFile(int i, int recsize){
    return i*(recsize + sizeof("\n"));
}

void shuffle(char *lib, char *file, char *recs, char *recsize) {
    int recnum = atoi(recs);
    int recsiz = atoi(recsize);
    char *rec1 = calloc(1, recsiz);
    char *rec2 = calloc(1, recsiz); 
    int j=0;
    if(recnum<=0 || recsiz <=0) return;
    if (strcmp(lib, "sys")==0) {
	int handle = open(file, O_RDWR);
	if (handle == -1) {
            printf("Cannot open file!");
            exit(1);
        }
	for(int i=0; i<recnum-1; i++){
	    j = i + rand()%(recnum-i);
	    lseek(handle, positionInFile(i, recsiz), SEEK_SET);
	    read(handle, rec1, (size_t) recsiz);
	    lseek(handle, positionInFile(j, recsiz), SEEK_SET);
	    read(handle, rec2, (size_t) recsiz);
	    lseek(handle, positionInFile(i, recsiz), SEEK_SET);
	    write(handle, rec2, (size_t) recsiz);
	    lseek(handle, positionInFile(j, recsiz), SEEK_SET);
	    write(handle, rec1, (size_t) recsiz);	    
	}
	free(rec1);
	free(rec2);
	close(handle);
    } else {
	FILE *handle = fopen(file, "r+");
	for(int i=0; i<recnum-1; i++){
	    j=i+rand()%(recnum-i);
	    fseek(handle, positionInFile(i, recsiz), SEEK_SET);
	    fread(rec1, sizeof(char), (size_t) recsiz, handle);
	    fseek(handle, positionInFile(j, recsiz), SEEK_SET);
	    fread(rec2, sizeof(char), (size_t) recsiz, handle);
	    fseek(handle, positionInFile(i, recsiz), SEEK_SET);
	    fwrite(rec2, sizeof(char), (size_t) recsiz, handle);
	    fseek(handle, positionInFile(j, recsiz), SEEK_SET);
	    fwrite(rec1, sizeof(char), (size_t) recsiz, handle);
	    fflush(handle);
	}
	free(rec1);
	free(rec2);
	fclose(handle);
    }
}

void generate(char *lib, char *file, char *recs, char *recsize) {
    int recnum = atoi(recs);
    int recsiz = atoi(recsize);
    if (strcmp(lib, "sys")==0) {
        int handle = open(file, O_WRONLY|O_CREAT|O_TRUNC);
        /*if (handle != -1) {
            perror("Such file already exists!");
            exit(1);
        }*/
	int random = open("/dev/urandom", O_RDONLY);
	if(random == -1){
	    perror("Can't open dev/urandom");
	    exit(1);
	}
        char MyRandomData[recsiz];
	size_t randomDataLen = 0;
	for(int i=0; i<recnum; i++){
	    randomDataLen = 0;
	    while(randomDataLen<sizeof(MyRandomData)){
    	        ssize_t result = read(random, MyRandomData + randomDataLen, sizeof(MyRandomData) - randomDataLen);
	        randomDataLen += result;
	    }
	    write(handle, MyRandomData, randomDataLen);
	    write(handle, "\n", sizeof("\n"));
	}
	close(random);
        close(handle);
    } else {
        FILE *handle = fopen(file, "a+");
        /*if (handle != NULL) {
            perror("Such file already exists!");
            exit(1);
        }
        handle = fopen(file, "a+");*/
	FILE *random = fopen("/dev/urandom", "r");
	if(random == NULL){
	    perror("Can't open dev/urandom");
	    exit(1);
	}
        char MyRandomData[recsiz];
	for(int i=0; i<recnum; i++){
	    fread(MyRandomData, sizeof(char), (size_t)recsiz, random);	 
	    fwrite(MyRandomData, sizeof(char), recsiz, handle);
	    fwrite("\n", sizeof(char), sizeof("\n"), handle);
	}
	fclose(random);
        fclose(handle);
    }
    chmod(file, S_IRWXU | S_IRWXG | S_IRWXO);
}

void sort(char *lib, char *file, char *recs, char *recsize) {
    int recnum = atoi(recs);
    int recsiz = atoi(recsize);
    char *rec1 = calloc(1, recsiz);
    char *rec2 = calloc(1, recsiz);
    if (strcmp(lib, "sys")) {
	int handle = open(file, O_RDWR);
	if(handle==-1){
	    perror("Can't open file");
	    exit(1);
	}
	int sorted = 0;
	while(sorted == 0){
	    sorted = 1;
	    for(int i=0; i<recnum-1; i++){
		lseek(handle, positionInFile(i, recsiz), SEEK_SET);
	        read(handle, rec1, (size_t) recsiz);
	        lseek(handle, positionInFile(i+1, recsiz), SEEK_SET);
	        read(handle, rec2, (size_t) recsiz);
		if(rec1[0]>rec2[0]){
		    lseek(handle, positionInFile(i, recsiz), SEEK_SET);
	            write(handle, rec2, (size_t) recsiz);
	            lseek(handle, positionInFile(i+1, recsiz), SEEK_SET);
	            write(handle, rec1, (size_t) recsiz);
		    sorted=0;
		}
	    }
	}
	close(handle);
	free(rec1);
	free(rec2);
    } else {
	FILE *handle = fopen(file, "w+");
	if(handle == NULL){
	    perror("Can't open file");
	    exit(1);
	}
	int sorted = 0;
	while(sorted == 0){
	    sorted = 1;
	    for(int i=0; i<recnum-1; i++){
		fseek(handle, positionInFile(i, recsiz), SEEK_SET);
	        fread(rec1, sizeof(char), (size_t) recsiz, handle);
	        fseek(handle, positionInFile(i+1, recsiz), SEEK_SET);
	        fread(rec2, sizeof(char), (size_t) recsiz, handle);
		if(rec1[0]>rec2[0]){
		    fseek(handle, positionInFile(i, recsiz), SEEK_SET);
	    	    fwrite(rec2, sizeof(char), (size_t) recsiz, handle);
	    	    fseek(handle, positionInFile(i+1, recsiz), SEEK_SET);
	    	    fwrite(rec1, sizeof(char), (size_t) recsiz, handle);
		    sorted=0;
		}
		fflush(handle);
	    }
	}
	fclose(handle);
	free(rec1);
	free(rec2);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 6)return 0;
    srand(time(NULL));
    if (strcmp(argv[2], "shuffle")==0) {
        shuffle(argv[1], argv[3], argv[4], argv[5]);
    } else if (strcmp(argv[2], "generate")==0) {
        generate(argv[1], argv[3], argv[4], argv[5]);
    } else if (strcmp(argv[2], "sort")==0) {
        sort(argv[1], argv[3], argv[4], argv[5]);
    }
    return 0;
}
