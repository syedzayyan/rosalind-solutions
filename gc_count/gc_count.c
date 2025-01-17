#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <common.h>

int main() {
    char *content = file_to_char("rosalind_gc.txt");
    if (content == NULL) {
        fprintf(stderr, "Failed to read file.\n");
        return 1;
    }

    double curr_seq_gc_content = 0.0;
    char *curr_id = NULL;

    while (content) {
        // Find the next '>' character (start of a new sequence)
        char *init_fasta = memchr(content, '>', strlen(content));
        if (init_fasta == NULL) {
            break;
        }
        content = init_fasta + 1;

        // Find the end of the identifier line (marked by '\n')
        char *init_seq = memchr(content, '\n', strlen(content));
        if (init_seq == NULL) {
            break;
        }

        // Gather the identifier
        char identifier[init_seq - content + 1];
        strncpy(identifier, content, init_seq - content);
        identifier[init_seq - content] = '\0';

        // Move content pointer to the start of the sequence data
        content = init_seq + 1;

        // Find the next '>' character (start of the next sequence)
        char *next_fasta = memchr(content, '>', strlen(content));
        size_t sequence_length;
        if (next_fasta) {
            sequence_length = next_fasta - content;  // Sequence length up to the next '>'
        } else {
            sequence_length = strlen(content);  // Sequence until the end of the file
        }

        // Allocate memory for the sequence and copy it, ignoring newlines
        char *sequence = (char *)malloc(sequence_length + 1);
        if (sequence == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }

        // Copy sequence data, removing newlines, because strcopy was causing problems with the GC calculation.
        char *seq_ptr = sequence;
        char *temp_content = content;
        for (size_t i = 0; i < sequence_length; i++) {
            if (*temp_content != '\n') {
                *seq_ptr = *temp_content;
                seq_ptr++;
            }
            temp_content++;
        }
        *seq_ptr = '\0';  // Null-terminate the sequence

        // Calculate GC content
        int gc_count = 0;
        seq_ptr = sequence;  // Reset pointer to the start of the sequence

        while (*seq_ptr) {
            if (*seq_ptr == 'G' || *seq_ptr == 'C') {
                gc_count++;
            }
            seq_ptr++;
        }

        double gc_content = (double)gc_count / strlen(sequence);

        if (gc_content > curr_seq_gc_content) {
            curr_seq_gc_content = gc_content;
            // Free previous curr_id if allocated
            if (curr_id != NULL) {
                free(curr_id);
            }
            // Allocate memory and store the current identifier
            curr_id = strdup(identifier);
        }

        // Clean up and prepare for the next iteration
        free(sequence);
        content = next_fasta;  // Move to the next sequence (or NULL if at the end)
    }

    printf("%s\n%f\n", curr_id, curr_seq_gc_content * 100);
    free(curr_id);  // Free the allocated memory for curr_id
    return 0;
}