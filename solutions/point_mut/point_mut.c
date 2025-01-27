#include <stdio.h>
#include "../common.h"
#include <stdlib.h>  // Include stdlib.h for malloc
#include <string.h>

int main() {
    char* content = file_to_char("rosalind_hamm.txt");
    // char *ptr = content;
    // while (*ptr) {

    // }
    char *seq_1 = NULL;
    char *seq_2 = NULL;

    char *pos_of_newline = strchr(content, '\n');
    if (pos_of_newline != NULL) {
        size_t len = pos_of_newline - content;
        seq_1 = (char *)malloc((len + 1) * sizeof(char));
        if (seq_1 != NULL){
            strncpy(seq_1, content, len);
            seq_1[len] = '\0';
        }
        seq_2 = strdup(pos_of_newline + 1);
        int distance;
        while (*seq_1 && *seq_2){
            if(*seq_1 != *seq_2){
                distance++;
            }
            seq_1++;
            seq_2++;
        }
        printf("%i\n", distance);
    } else {
        free(content);
        return 1;
    }

    free(content);
    return 0;
}