#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>
#include <ctype.h>

#define MAX_WORDS 255
#define MAX_ARGVS 50
#define MAX_ARGS 50


typedef struct Node {
    char* path;
    struct Node* next;
} Node;

char error_message[30] = "An error has occurred\n";

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

char* redirect(char** args, int argn, int* errs) {
    char* redirectTo = NULL;
    for (int i = 0; i < argn; i++) {
        if (strcmp(args[i], ">") == 0) {
            if (i + 2 == argn) {
                redirectTo = args[i + 1];
                args[i] = NULL;
                args[i + 1] = NULL;
                break;
            } else {
                write(STDERR_FILENO, error_message, strlen(error_message)); 
                *errs = 1;
                break;
            }
        }
    }
    return redirectTo;
}

int is_only_whitespace(char* line) {
    for (int i = 0; line[i]; i++) {
        if (!isspace((unsigned char)line[i])) {
            return 0;
        }
    }
    return 1;
}

void split_argV(char** argV, int argC, int* argVsC, char* argVs[MAX_ARGVS][MAX_ARGS]) {
    int count = 0;
    int innerCount = 0;

    for (int i = 0; i < argC; i++) {
        if (strcmp(argV[i], "&") == 0) {
            argVs[count][innerCount] = NULL;
            count++;
            innerCount = 0;
        } else {
            argVs[count][innerCount] = argV[i];
            innerCount++;
        }
    }
    argVs[count][innerCount] = NULL;
    *argVsC = count + 1;
}

int main(int argc, char *argv[]) {
    // Check for arguments
    if (argc > 1) {
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        return 1;
    }

    // Define rushPATH
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
        
        // If the line is empty, skip it and print the prompt again
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }
        fflush(stdout);
        
        // Remove the newline character from the end of the line
        line[strcspn(line, "\n")] = 0;
        
        // If the line is only whitespace, skip it and print the prompt again
        if (is_only_whitespace(line)) {
            continue;
        }

        // Split the line into words
        int wordcount = 0;
        char **words = get_words(line, &wordcount);
        free(line);
        
        // Split the words into commands
        int numOfCommands = 0;
        char* Commands[MAX_ARGVS][MAX_ARGS];
        split_argV(words, wordcount, &numOfCommands, Commands);

        // print commands
        // printf("argVsC: %d\n", numOfCommands);
        // for (int i = 0; i < numOfCommands; i++) {
        //     for (int j = 0; Commands[i][j] != NULL; j++) {
        //         printf("%d-%d - %s\n", i, j, Commands[i][j]);
        //     }
        //     printf("\n");
        // }

        // make an array of pids
        pid_t pids[numOfCommands];

        // for each of the commands, execute them
        for (int i = 0; i < numOfCommands; i++) {
            char** argV = Commands[i];
            if(argV[0] == NULL) {
                continue;
            }

            // coutn the number of stings in argV and assign it to argC
            int argC = 0;
            for (int j = 0; argV[j] != NULL; j++) {
                argC++;
            }

            // exit case
            if (strcmp(argV[0], "exit") == 0) {
                if (argC > 1) {
                    write(STDERR_FILENO, error_message, strlen(error_message)); 
                } else {
                    exit(0);
                }
            }

            // cd case
            else if (strcmp(argV[0], "cd") == 0) {
                if (argC == 2) {
                    int result = chdir(argV[1]);
                    if (result != 0) {
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                    }
                } else {
                    write(STDERR_FILENO, error_message, strlen(error_message)); 
                }
                continue;
            }
            
            // path case
            else if (strcmp(argV[0], "path") == 0) {
                // Free the old path
                Node* current = rushPATH;
                while (current != NULL) {
                    Node* next = current->next;
                    free(current->path);
                    free(current);
                    current = next;
                }

                // Initialize the new path
                rushPATH = NULL;
                pathLength = 0;

                // Add the new paths
                for (int i = 1; i < argC; i++) {
                    rushPATH = addPath(rushPATH, argV[i], &pathLength);
                }
            }


            // all other cases
            else{
                for (Node* node = rushPATH; node != NULL; node = node->next) {
                    char fullPath[256];
                    snprintf(fullPath, sizeof(fullPath), "%s/%s", node->path, argV[0]); 
                    int result = access(fullPath, X_OK);
                    if (result == 0) {
                        pids[i] = fork();
                        if (pids[i] < 0) {
                            printf("%d %d\n", i, pids[i]);
                            write(STDERR_FILENO, error_message, strlen(error_message)); 
                        }
                        else if (pids[i] == 0) {
                            // Child process
                            char* redirectTo = redirect(argV, argC, &errs);
                            if (errs > 0) {
                                exit(1);
                            }

                            if(redirectTo != NULL) {
                                freopen(redirectTo, "w", stdout);
                            }

                            execv(fullPath, argV);
                            // If execv returns, there was an error
                            write(STDERR_FILENO, error_message, strlen(error_message)); 
                            exit(0);
                        }
                    }
                    else{
                        continue;
                    }
                }
            
            }

            for (int i = 0; i < numOfCommands; i++) {
                int status;
                waitpid(pids[i], &status, 0);
            }
        }

    }
    return 0;
}