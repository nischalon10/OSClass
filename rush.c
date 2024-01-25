#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    while (1) {
        char *line = NULL;
        size_t len = 0;
        printf("Enter a line: ");
        getline(&line, &len, stdin);

        char *words[MAX_WORDS];
        int word_count = 0;

        char *token;
        char *rest = line;
        while ((token = strsep(&rest, " ")) != NULL && word_count < MAX_WORDS) {
            words[word_count] = malloc(strlen(token) + 1);
            strcpy(words[word_count], token);
            word_count++;
        }

        // Print the words to verify
        for (int i = 0; i < word_count; i++) {
            printf("%s\n", words[i]);
            free(words[i]);
        }

        free(line);
    }
    return 0;
}