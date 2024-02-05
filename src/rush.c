#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>
#include <ctype.h>

#define MAX_WORDS 255

char error_message[30] = "An error has occurred\n";

typedef struct Node {
    char* path;
    struct Node* next;
} Node;

Node* addPath(Node* head, char* newPath, int* pathLength) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->path = strdup(newPath);
    node->next = NULL;

    if (head == NULL) {
        head = node;
    } else {
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }

    (*pathLength)++;
    return head;
}

char** get_words(char* line, int* argC) {
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

int main(int argc, char *argv[]) {
    if (argc > 1) {
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        return 1;
    }

    Node* rushPATH = NULL;
    int pathLength = 0;
    rushPATH = addPath(rushPATH, "/bin", &pathLength);
    
    // Start the RUSH shell 
    while(1){
        int errs = 0;
        char *line = NULL;
        size_t len = 0;

        fflush(stdout);
        printf("rush> ");
        getline(&line, &len, stdin);
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }
        fflush(stdout);
        
        line[strcspn(line, "\n")] = 0;
        
        int onlyWhitespace = 1;
        for (int i = 0; line[i]; i++) {
            if (!isspace((unsigned char)line[i])) {
                onlyWhitespace = 0;
                break;
            }
        }
        if (onlyWhitespace) {
            continue;
        }

        int numWords = 0;
        char **words = get_words(line, &numWords);
        free(line);

        char **cmds[MAX_WORDS];
        int cmdCount = 0;
        int argCount = 0;
        cmds[cmdCount] = &words[argCount];
        for (int i = 0; words[i] != NULL; i++) {
            if (strcmp(words[i], "&") == 0) {
                words[i] = NULL;  // End the current command
                cmds[++cmdCount] = &words[i + 1];  // Start a new command
            }
        }

        for (int i = 0; i <= cmdCount; i++) {
            char **argV = cmds[i];
            printf("[");

            for (int j = 0; argV[j] != NULL; j++) {
                printf("'%s'", argV[j]);
                if (argV[j + 1] != NULL) {
                    printf(", ");
                }
            }
            printf("]\n");
        }

        for(int i = 0; i < cmdCount; i++) {
            
        }

        // check if argV has '>' and save the file name in a variable and clear everything after it
    //     char* redirectTo = NULL;
    //     for (int i = 0; i < argC; i++) {
    //         if (strcmp(argV[i], ">") == 0) {
    //             if (i + 2 == argC) {
    //                 redirectTo = argV[i + 1];
    //                 argV[i] = NULL;
    //                 argV[i + 1] = NULL;
    //                 break;
    //             } else {
    //                 write(STDERR_FILENO, error_message, strlen(error_message)); 
    //                 errs = 1;
    //                 break;
    //             }
    //         }
    //     }
    //     if (errs > 0) {
    //         continue;
    //     }
    //     // exit case
    //     if (strcmp(argV[0], "exit") == 0) {
    //         if (argC > 1) {
    //             write(STDERR_FILENO, error_message, strlen(error_message)); 
    //         } else {
    //             exit(0);
    //         }
    //     }
    //     // cd case
    //     else if (strcmp(argV[0], "cd") == 0) {
    //         if (argC == 2) {
    //             int result = chdir(argV[1]);
    //             if (result != 0) {
    //                 write(STDERR_FILENO, error_message, strlen(error_message)); 
    //             }
    //         } else {
    //             write(STDERR_FILENO, error_message, strlen(error_message)); 
    //         }
    //         continue;
    //     }
    //     // path case
    //     else if (strcmp(argV[0], "path") == 0) {
    //         // Free the old path
    //         Node* current = rushPATH;
    //         while (current != NULL) {
    //             Node* next = current->next;
    //             free(current->path);
    //             free(current);
    //             current = next;
    //         }

    //         // Initialize the new path
    //         rushPATH = NULL;
    //         pathLength = 0;

    //         // Add the new paths
    //         for (int i = 1; i < argC; i++) {
    //             rushPATH = addPath(rushPATH, argV[i], &pathLength);
    //         }
    //     }
    //     else{
    //     // all other cases
    //         for (Node* node = rushPATH; node != NULL; node = node->next) {
    //             char fullPath[256];
    //             snprintf(fullPath, sizeof(fullPath), "%s/%s", node->path, argV[0]); 
    //             int result = access(fullPath, X_OK);
    //             if (result == 0) {
    //                 pid_t pid = fork();
    //                 if (pid < 0) {
    //                     write(STDERR_FILENO, error_message, strlen(error_message)); 
    //                 } 
    //                 else if (pid == 0) {
    //                     fflush(stdout);
    //                     // Child process
    //                     if(redirectTo != NULL) {
    //                         freopen(redirectTo, "w", stdout);
    //                     }
    //                     execv(fullPath, argV);
    //                     // If execv returns, there was an error
    //                     write(STDERR_FILENO, error_message, strlen(error_message)); 
    //                     exit(1);
    //                 } else {
    //                     // Parent process
    //                     fflush(stdout);
    //                     int status;
    //                     waitpid(pid, &status, 0);
    //                 }
    //                 break;
    //             }
    //         }
    //     }
    //     for (int i = 0; i < argC; i++) {
    //         free(argV[i]);
    //     }
    }      
    return 0;
}