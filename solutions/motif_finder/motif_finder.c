#include <stdio.h>
#include <string.h>
#include <common.h>
#include <stdlib.h>

int main() {
    char *str = file_to_char("rosalind_subs.txt");
    char *newline_pos = memchr(str, '\n', strlen(str));

    if (newline_pos == NULL) {
        printf("Newline character not found in the string.\n");
        free(str);
        return 1;
    }

    size_t first_half_length = newline_pos - str;
    char *first_half = malloc(first_half_length + 1);
    strncpy(first_half, str, first_half_length);
    first_half[first_half_length] = '\0';

    char *second_half = strdup(newline_pos + 1);

    int index = 0;
    char *search_pos = first_half;
    char *found;

    while ((found = strstr(search_pos, second_half)) != NULL) {
        index = found - first_half + 1;
        printf("%d ", index);
        search_pos = found + 1;
    }
    printf("\n");

    free(first_half);
    free(second_half);
    free(str);

    return 0;
}