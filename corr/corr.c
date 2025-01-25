#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common.h"

#define MAX_READS 1000
#define MAX_LENGTH 50


int main() {
    const char *filename = "rosalind_corr.txt";
    int num_sequences = 0;
    char **reads = read_fasta_to_array(filename, &num_sequences);

    // Count occurrences of each read and its reverse complement
    char rev_comp[MAX_LENGTH + 1];
    int counts[MAX_READS] = {0};

    for (int i = 0; i < num_sequences; i++) {
        counts[i] = 1;
        reverse_complement(reads[i], rev_comp);
        for (int j = i + 1; j < num_sequences; j++) {
            if (strcmp(reads[i], reads[j]) == 0 || strcmp(rev_comp, reads[j]) == 0) {
                counts[i]++;
            }
        }
    }

    // Identify corrections
    for (int i = 0; i < num_sequences; i++) {
        if (counts[i] == 1) {  // Potential incorrect read
            for (int j = 0; j < num_sequences; j++) {
                if (i != j && counts[j] > 1) {  // Check against correct reads
                    if (hamm(reads[i], reads[j]) == 1) {
                        printf("%s->%s\n", reads[i], reads[j]);
                        break;
                    }
                    reverse_complement(reads[j], rev_comp);
                    if (hamm(reads[i], rev_comp) == 1) {
                        printf("%s->%s\n", reads[i], rev_comp);
                        break;
                    }
                }
            }
        }
    }

    // Clean up
    for (int i = 0; i < num_sequences; i++) {
        free(reads[i]);
    }
    free(reads);

    return 0;
}
