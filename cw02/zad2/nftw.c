#define _XOPEN_SOURCE 500
#define _BSD_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ftw.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int maxsize;

void printtime(struct tm *val){
    printf("%d.%d.%d\n", val->tm_mday, (val->tm_mon)+1, (val->tm_year)+1900);
}

int nftwfunc(const char *path, const struct stat *buffer, int info, struct FTW *str){
    if(buffer->st_size>maxsize) return 0;
	char *abs_path = calloc(1000, sizeof(char));
    //printf("Object path: %s \n", path);
    //printf("Object size: %d \n", (int)buffer->st_size);
    char *permissions = calloc(10, sizeof(char));
    if(buffer->st_mode & S_IRUSR) permissions[0]='r';
    else permissions[0]='-';
    if(buffer->st_mode & S_IWUSR) permissions[1]='w';
    else permissions[1]='-';
    if(buffer->st_mode & S_IXUSR) permissions[2]='x';
    else permissions[2]='-';
    if(buffer->st_mode & S_IRGRP) permissions[3]='r';
    else permissions[3]='-';
    if(buffer->st_mode & S_IWGRP) permissions[4]='w';
    else permissions[4]='-';
    if(buffer->st_mode & S_IXGRP) permissions[5]='x';
    else permissions[5]='-';
    if(buffer->st_mode & S_IROTH) permissions[6]='r';
    else permissions[6]='-';
    if(buffer->st_mode & S_IWOTH) permissions[7]='w';
    else permissions[7]='-';
    if(buffer->st_mode & S_IXOTH) permissions[8]='x';
    else permissions[8]='-';
    permissions[9]='\0';
	realpath(path, abs_path);
    printf("%s %d %s", abs_path, (int)(buffer->st_size), permissions);
    printtime(localtime(&buffer->st_mtime));
    free(permissions);
	free(abs_path);
    return 0;
}

int main(int argc, char* argv[]){
    if(argc<3){
	perror("Not enough arguments!");
	exit(1);
    }
    char* path = calloc(strlen(argv[1])+1, sizeof(char));
    strcpy(path, argv[1]);
    maxsize = atoi(argv[2]);
    nftw(path, nftwfunc, 5, FTW_DEPTH | FTW_PHYS);
    free(path);
    return 0;
}
