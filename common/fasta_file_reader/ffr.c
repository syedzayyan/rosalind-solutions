#include "../../common.h"

hashmap* fasta_file_reader(const char *filename) {
    char *fast_files = file_to_char(filename);
    if (fast_files == NULL) {
        fprintf(stderr, "Failed to read file.\n");
        return NULL;
    }

    // Create the hashmap
    hashmap* map = hashmap_create();
    if (map == NULL) {
        fprintf(stderr, "Failed to create hashmap.\n");
        free(fast_files);
        return NULL;
    }

    char *content = fast_files;    

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
            free(content);
            hashmap_delete(map);
            return NULL;
        }

        // Copy sequence data, removing newlines
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

        // Store the identifier and sequence in the hashmap
        hashmap_set(map, identifier, sequence, sequence_length + 1);

        // Move to the next sequence
        content = next_fasta;
    }

    return map;
}
