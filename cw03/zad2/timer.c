#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
	int i=0;
    while(1){
		i++;
		i--;
	}
    return 0;
}
