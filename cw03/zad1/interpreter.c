#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Can't open file\n");
        exit(1);
    }
    int status;
    int maxlength = 128;
    char *line = calloc(maxlength, sizeof(char));
    char *word = calloc(maxlength, sizeof(char));
    while ((line = fgets(line, maxlength, file)) != NULL) {
        word = strtok(line, " \n");
        if (word == NULL || word[0]==' ') continue;
        if (word[0] == '#') {
            char *name = calloc(strlen(word) - 1, sizeof(char));
            memcpy(name, word + 1, strlen(word) - 1);
            char *tmp = strtok(NULL, " \n");
            if (tmp == NULL) {
                unsetenv(name);
            } else {
                setenv(name, tmp, 1);
            }
            free(name);
        } else {
            char **arr = calloc(1, sizeof(char *));
            arr[0] = word;
            char *tmp;
            int i = 1;
            do {
				i++;
                arr = realloc(arr, i*sizeof(char*));
                tmp = strtok(NULL, " \n");
                if (tmp == NULL) arr[i - 1] = NULL;
                else if (getenv(tmp) != NULL) {
                    arr[i - 1] = calloc(strlen(getenv(tmp)), sizeof(char));
                    arr[i - 1] = getenv(tmp);
                } else {
                    arr[i - 1] = calloc(strlen(tmp), sizeof(char));
                    arr[i - 1] = tmp;
                }
            } while (tmp != NULL);
            int pid = fork();
            if (pid < 0) printf("Process cannot be created\n");
            else if (pid == 0) {
                execvp(arr[0], arr);
                exit(0);
            } else {
                wait(&status);
                if (status != 0) printf("Error ocured during executing a command\n");
            }
            free(tmp);
            //for (int j = 0; j < i - 1; j++) {
            //    if (arr[j] != NULL) free(arr[j]);
            //}
            
            free(arr);
        }

    }
    fclose(file);
    return 0;
}
