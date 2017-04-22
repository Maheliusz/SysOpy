#define _XOPEN_SOURCE 500
#define _BSD_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int maxsize;

void printtime(struct tm *val){
    printf("%d.%d.%d\n", val->tm_mday, val->tm_mon, (val->tm_year)+1900);
}

void printinfo(const char *path, const struct stat *buffer){
    if(buffer->st_size>maxsize) return;
    char *tmp = calloc(1000, sizeof(char));
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
    realpath(path, tmp);
    printf("%s %d %s", tmp, (int)(buffer->st_size), permissions);
    printtime(localtime(&buffer->st_mtime));
    free(tmp);
    free(permissions);
}

void search(char *path){
    DIR *directory = opendir(path);
    if(directory == NULL){
        perror("Can't open directory");
	exit(1);
    }
    char *newdir = calloc(1000, sizeof(char));
    struct dirent *contents;
    struct stat buf;
    /*while((contents = readdir(directory))!=NULL){
		if(strcmp(contents->d_name, ".")!=0 &&  strcmp(contents->d_name, "..")!=0){
			strcpy(newdir, path);
			strcat(newdir, contents->d_name);
			strcat(newdir, "/");
			search(newdir);
		}
		else{
	    	strcpy(newdir, path);
	    	strcat(newdir, contents->d_name);
	    	stat(newdir, buf);
	    	printinfo(newdir, buf);
		}
    } */
    while((contents=readdir(directory))!=NULL){
    	snprintf(newdir, 1000, "%s/%s", path, contents->d_name);
    	lstat(newdir, &buf);
    	if(S_ISDIR(buf.st_mode)){
    		if(strcmp(contents->d_name, ".")!=0 &&  strcmp(contents->d_name, "..")!=0){
				int len;
				char dirpath[1000];
				len = snprintf(dirpath, 1000, "%s/%s", path, contents->d_name);
				if(len<1000){
					dirpath[len] = '\0';
					search(dirpath);
				}
			}
    	}else if(S_ISREG(buf.st_mode)){
    		if(buf.st_size<maxsize){
    			printinfo(newdir, &buf);
    		}
    	}
    }
    closedir(directory);
}

int main(int argc, char *argv[]){
    if(argc<3){
		perror("Not enough arguments!");
		exit(1);
    }
    char* cat = calloc(1000, sizeof(char));
    strcpy(cat, argv[1]);
    printf("-%s-\n", cat);
	chdir(cat);
	getcwd(cat, 1000);
    maxsize = atoi(argv[2]);
    search(cat);
    return 0;
}
