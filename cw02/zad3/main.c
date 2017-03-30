#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>

void printinfo(){
    printf("Struktura opcji:\n");
    printf("1 1 [numer] - ustawienie rygla do odczytu na wybrany znak pliku, wersja nieblokująca\n");
    printf("1 2 [numer] - ustawienie rygla do odczytu na wybrany znak pliku, wersja blokująca\n");
    printf("2 1 [numer] - ustawienie rygla do zapisu na wybrany znak pliku, wersja nieblokująca\n");
    printf("2 2 [numer] - ustawienie rygla do zapisu na wybrany znak pliku, wersja blokująca\n");
    printf("3 - wyświetlenie zaryglowanych znaków pliku\n");
    printf("4 [numer] - zwolnienie wybranego rygla\n");
    printf("5 [numer] - odczyt wybranego znaku pliku\n");
    printf("6 [numer] - zmiana wybranego znaku pliku\n");
    printf("7 - opuszczenie interpretera\n");
}

struct flock *setlock(int byte, int type){
    struct flock *lock = calloc(1, sizeof(struct flock));
    lock->l_type = type;
    lock->l_len = 1;
    lock->l_whence = SEEK_SET;
    lock->l_start = byte;
    return lock;
}

void setread(int byte, int file, int block){
    struct flock *lock = setlock(file, F_RDLCK);
    if(block==0){
	if(fcntl(file, F_SETLK, lock) == -1){
	    printf("Lock cannot be set\n");
	    free(lock);
	}
    }
    else{
	if(fcntl(file, F_SETLKW, lock) == -1){
	    printf("Lock cannot be set\n");
	    free(lock);
	}
    }
}

void setwrite(int byte, int file, int block){
    struct flock *lock = setlock(file, F_WRLCK);
    if(block==0){
	if(fcntl(file, F_SETLK, lock) == -1){
	    printf("Lock cannot be set\n");
	    free(lock);
	}
    }
    else{
	if(fcntl(file, F_SETLKW, lock) == -1){
	    printf("Lock cannot be set\n");
	    free(lock);
	}
    }
}

void releaselock(int byte, int file){
    struct flock *lock = setlock(byte, F_UNLCK);
    if(fcntl(file, F_SETLK, lock) == -1){
        printf("Lock cannot be released\n");
        free(lock);
    }
}

void readbyte(int byte, int file){
    lseek(file, byte, SEEK_SET);
    char sign;
    if(read(file, &sign, 1)<=0) printf("Error on reading file\n");
    else printf("Character read: %c\n", sign);
}

void writebyte(int byte, int file, char content){
    lseek(file, byte, SEEK_SET);
    if(write(file, &content, 1)<=0) printf("Error on writing file\n");
    else printf("Character written\n");
}

void printlocks(int file){
    struct flock *lock = setlock(0, F_WRLCK);
    int size = lseek(file, 0, SEEK_END);
    for(int i=0; i<size; i++){
	lock->l_len = 1;
	lock->l_whence = SEEK_SET;
	lock->l_type = F_WRLCK;
	lock->l_start = i;
	fcntl(file, F_GETLK, lock);
	if(lock->l_type==F_RDLCK) 
	    printf("Read lock on byte %d, PID: %d \n", i, lock->l_pid);
	else if(lock->l_type==F_WRLCK) 
	    printf("Write lock on byte %d, PID: %d \n", i, lock->l_pid);
    }
}

int main(int argc, char *argv[]){
    if(argc<2) return 0;
    char *file = calloc(sizeof(argv[1]), sizeof(char));
    strcpy(file, argv[1]);
    int handle = open(file, O_RDWR);
    char *input = calloc(128, sizeof(char));
    printinfo();
    while(1){
	fgets(input, 128, stdin);
	if(input[0]-'0'==1){
	    if(strlen(input)<5) continue;
	    char *substr = calloc(sizeof(input)-4, sizeof(char));
	    memcpy(substr, input+4, strlen(input)-1);
	    int target = atoi(substr);
	    if(input[2]-'0'==1){
		setread(target, handle, 0);
	    }
	    else{
		setread(target, handle, 1);
	    }
	    free(substr);
	}
	else if(input[0]-'0'==2){
	    if(strlen(input)<5) continue;
    	    char *substr = calloc(strlen(input)-4, sizeof(char));
	    memcpy(substr, input+4, strlen(input)-1);
	    int target = 0;
	    target = atoi(substr);
	    if(input[2]-'0'==1){
		setwrite(target, handle, 0);
	    }
	    else{
		setwrite(target, handle, 1);
	    }
	    free(substr);
	}
	else if(input[0]-'0'==3){
	    printlocks(handle);
	}
	else if(input[0]-'0'==4){
	    if(strlen(input)<3) continue;
	    char *substr = calloc(strlen(input)-2, sizeof(char));
	    memcpy(substr, input+2, strlen(input)-1);
	    int target = 0;
	    target = atoi(substr);
	    releaselock(target, handle);
	    free(substr);
	}
	else if(input[0]-'0'==5){
	    if(strlen(input)<3) continue;
	    char *substr = calloc(strlen(input)-2, sizeof(char));
	    memcpy(substr, input+2, strlen(input)-1);
	    int target = 0;
	    target = atoi(substr);
	    readbyte(target, handle);
	    free(substr);
	}
	else if(input[0]=='6'){
	    if(strlen(input)<5) continue;
	    char *substr = calloc(strlen(input)-2, sizeof(char));
	    //memcpy(substr, input[2], strlen(input)-1);
	    int i;
	    for(i=2; input[i]!=' ' && input[i]!='\0'; i++){
		substr[i-2] = input[i];
	    }
	    substr[i]='\0';
	    int target = 0;
	    target = atoi(substr);
	    writebyte(target, handle, input[i+1]);
	    free(substr);
	}
	else if(input[0]-'0'==7) break;
    }
    free(input);
    return 0;
}
