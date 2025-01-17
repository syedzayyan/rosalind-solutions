#include <stdio.h>
#include <common.h>
#include <stdlib.h>  // Include stdlib.h for malloc

int main() {
    char* content = file_to_char("rosalind_dna.txt");

    int A = 0;
    int C = 0;
    int G = 0;
    int T = 0;

    char *ptr = content;
    while (*ptr) {
        switch (*ptr) {
            case 'A':
                A++;
                break;
            case 'C':
                C++;
                break;
            case 'G':
                G++;
                break;
            case 'T':
                T++;
                break;
            default:
                break;
        }
        ptr++;
    }

    printf("%i %i %i %i\n", A, C, G, T);

    free(content);
    return 0;
}