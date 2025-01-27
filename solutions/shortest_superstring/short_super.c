#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common.h"

// Function to find the overlap length between two strings
int overlap(const char* s1, const char* s2) {
    int max_overlap = 0;
    int max_len = strlen(s1) < strlen(s2) ? strlen(s1) : strlen(s2);

    for (int i = 1; i <= max_len; i++) {
        if (strncmp(s1 + strlen(s1) - i, s2, i) == 0) {
            max_overlap = i;
        }
    }
    return max_overlap;
}

// Function to merge two strings based on their overlap
char* merge(const char* s1, const char* s2) {
    int overlap_length = overlap(s1, s2);
    int merged_len = strlen(s1) + strlen(s2) - overlap_length;

    char* merged = malloc((merged_len + 1) * sizeof(char));
    strcpy(merged, s1);
    strcpy(merged + strlen(s1), s2 + overlap_length);

    return merged;
}

// Function to compute the shortest superstring from a list of sequences
char* shortest_superstring(char** reads, int num_reads) {
    while (num_reads > 1) {
        int max_overlap = -1;
        int best_i = -1, best_j = -1;
        char* best_merged = NULL;

        // Try merging every pair of reads
        for (int i = 0; i < num_reads; i++) {
            for (int j = 0; j < num_reads; j++) {
                if (i != j) {
                    int overlap_len = overlap(reads[i], reads[j]);
                    if (overlap_len > max_overlap) {
                        max_overlap = overlap_len;
                        best_i = i;
                        best_j = j;
                        best_merged = merge(reads[i], reads[j]);
                    }
                }
            }
        }

        // Remove the two best reads and add their merged result
        char** new_reads = malloc((num_reads - 1) * sizeof(char*));
        int new_idx = 0;
        for (int i = 0; i < num_reads; i++) {
            if (i != best_i && i != best_j) {
                new_reads[new_idx++] = reads[i];
            }
        }
        new_reads[new_idx] = best_merged;

        // Free the old reads and update the read list
        free(reads[best_i]);
        free(reads[best_j]);
        free(reads);
        reads = new_reads;
        num_reads--;
    }

    return reads[0];
}

int main() {
    const char* filename = "rosalind_long.txt"; // Path to your FASTA file
    int num_sequences = 0;
    char** reads = read_fasta_to_array(filename, &num_sequences);

    if (reads == NULL) {
        return 1;
    }

    char* result = shortest_superstring(reads, num_sequences);
    printf("%s\n", result);

    // Free the remaining sequences
    for (int i = 0; i < num_sequences; i++) {
        free(reads[i]);
    }
    free(reads);
    free(result);

    return 0;
}
