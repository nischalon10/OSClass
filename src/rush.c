#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>

#define MAX_WORDS 100

char** get_argV(char* line, int* argC) {
    char **argV = malloc(MAX_WORDS * sizeof(char*));
    *argC = 0;

    char *token;
    char *rest = line;
    while ((token = strsep(&rest, " \t")) != NULL && *argC < MAX_WORDS) {
        if (strlen(token) > 0) { // Ignore empty tokens
            argV[*argC] = malloc(strlen(token) + 1);
            strcpy(argV[*argC], token);
            (*argC)++;
        }
    }
    return argV;
}

int main() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    char *path[] = {"/bin", "/usr/bin", NULL};
    while(1){
        char *line = NULL;
        size_t len = 0;

        printf("rush> ");
        fflush(stdout);
        getline(&line, &len, stdin);
        line[strcspn(line, "\n")] = 0;

        int argC;
        char **argV = get_argV(line, &argC);
        free(line);
        // Print words        
        for (int i = 0; i < argC; i++) {
            printf("%d %s\n",i, argV[i]);
        }
        printf("word_count: %d\n", argC);
        printf("\n");

        // exit case
        if(strcmp(argV[0], "exit") == 0){
            free(argV);
            printf("\n");
            break;
        }
        // ls case
        else if(strcmp(argV[0], "ls") == 0){
            for (int i = 0; path[i] != NULL; i++){
                char fullPath[256];  // Adjust the size accordingly
                snprintf(fullPath, sizeof(fullPath), "%s/%s", path[i], argV[0]); 
                // printf("%s/%s\n", path[i], argV[0]);
                int result = access(fullPath, X_OK);
                if (result == 0) {
                    printf("Access Result:\t%d\n", result);
                    printf("Path: %s\n", fullPath);
                    break;
                }
            }
            // pid_t pid = fork();
            // if (pid < 0) {
            //     // Fork failed
            //     printf("Fork failed.\n");
            // } 

            // else if (pid == 0) {
            //     // Child process
            //     execv("/bin/ls", argV);

            //     // If execv returns, there was an error
            //     printf("execv failed.\n");
            //     exit(1);
            // } else {
            //     // Parent process
            //     int status;
            //     waitpid(pid, &status, 0);
            // }
        }

        printf("\n");
    }      
    return 0;
}