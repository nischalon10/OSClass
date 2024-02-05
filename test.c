#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *argV[] = {"cmd1", "&", "cmd2", "arg1", "&", "cmd3", "arg2", NULL};
    char **cmds[10];
    int cmdCount = 0;
    int argCount = 0;

    cmds[cmdCount] = &argV[argCount];
    for (int i = 0; argV[i] != NULL; i++) {
        if (strcmp(argV[i], "&") == 0) {
            argV[i] = NULL;  // End the current command
            cmds[++cmdCount] = &argV[i + 1];  // Start a new command
        }
    }

    // Print the commands
    for (int i = 0; i <= cmdCount; i++) {
        printf("[");
        for (int j = 0; cmds[i][j] != NULL; j++) {
            printf("'%s'", cmds[i][j]);
            if (cmds[i][j + 1] != NULL) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    return 0;
}