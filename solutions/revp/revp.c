#include "../../common.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 1000

// Function to find reverse palindromes in a DNA sequence
void find_reverse_palindromes(const char* sequence) {
    int seq_length = strlen(sequence);

    for (int start = 0; start < seq_length; start++) {
        for (int length = 4; length <= 12; length++) {
            if (start + length > seq_length) break; // Ensure substring is within bounds
            
            char sub_str[MAX_LEN], rev_comp[MAX_LEN];
            strncpy(sub_str, sequence + start, length);
            sub_str[length] = '\0';

            reverse_complement(sub_str, rev_comp);

            if (strcmp(sub_str, rev_comp) == 0) {
                printf("%d %d\n", start + 1, length); // 1-based indexing
            }
        }
    }
}

int main() {
    // Example DNA sequence
    char sequence[MAX_LEN];

    // Read input (FASTA format assumed to be stored in "rosalind_revp.txt")
    FILE* file = fopen("rosalind_revp.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Skip the FASTA header line (assuming it's the first line)
    fgets(sequence, MAX_LEN, file);

    // Read the actual DNA sequence
    sequence[0] = '\0';
    while (fgets(sequence + strlen(sequence), MAX_LEN - strlen(sequence), file));
    
    fclose(file);

    // Find reverse palindromes in the sequence
    find_reverse_palindromes(sequence);

    return 0;
}
