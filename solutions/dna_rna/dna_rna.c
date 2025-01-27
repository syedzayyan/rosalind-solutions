#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <common.h>

int main() {
    // Let's assume you get a pointer to a string, e.g., from a function
    const char* original = file_to_char("rosalind_rna.txt");

    char* content = malloc(strlen(original) + 1);  // +1 for the null terminator
    strcpy(content, original);

    char *ptr = content;
    while (*ptr) {
        if (*ptr == 'T') {
            *ptr = 'U';
        }
        ptr++;
    }

    printf("%s\n", content);

    free(content);
    free(original);
    return 0;
}
