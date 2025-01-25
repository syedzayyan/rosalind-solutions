#include <stdio.h>
#include <stdlib.h>  // Include stdlib.h for malloc
#include "common.h"
#include <string.h>

#define MAX_READS 1000
#define MAX_LENGTH 50

char* file_to_char(const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the file in read mode
    if (file == NULL) {
        printf("Could not open file.\n");
        return NULL;  // Return NULL on failure
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc((fileSize + 1) * sizeof(char));
    if (content == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;  // Return NULL on failure
    }

    fread(content, sizeof(char), fileSize, file);
    content[fileSize] = '\0';  // Null-terminate the string
    
    fclose(file);
    return content;  // Return the content
}

// Function to read a FASTA file and return an array of DNA sequences
char **read_fasta_to_array(const char *filename, int *num_sequences) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", filename);
        return NULL;
    }

    char **reads = (char **)malloc(MAX_READS * sizeof(char *));
    if (reads == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    char line[MAX_LENGTH + 2];  // Accommodate newline and null terminator
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < MAX_READS) {
        // Skip header lines (starting with '>')
        if (line[0] == '>') {
            continue;
        }

        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        reads[count] = strdup(line);  // Copy the sequence into the array
        if (reads[count] == NULL) {
            printf("Memory allocation failed for read %d.\n", count);
            fclose(file);
            return NULL;
        }
        count++;
    }

    fclose(file);
    *num_sequences = count;  // Set the number of sequences
    return reads;
}


hm* init_codon_map(){
    hm* codon_map = hm_create();
    
    hm_set(codon_map, "UUU", "F");
    hm_set(codon_map, "UUC", "F");
    hm_set(codon_map, "UUA", "L");
    hm_set(codon_map, "UUG", "L");
    hm_set(codon_map, "UCU", "S");
    hm_set(codon_map, "UCC", "S");
    hm_set(codon_map, "UCA", "S");
    hm_set(codon_map, "UCG", "S");
    hm_set(codon_map, "UAU", "Y");
    hm_set(codon_map, "UAC", "Y");
    hm_set(codon_map, "UGU", "C");
    hm_set(codon_map, "UGC", "C");
    hm_set(codon_map, "UGG", "W");
    hm_set(codon_map, "CUU", "L");
    hm_set(codon_map, "CUC", "L");
    hm_set(codon_map, "CUA", "L");
    hm_set(codon_map, "CUG", "L");
    hm_set(codon_map, "CCU", "P");
    hm_set(codon_map, "CCC", "P");
    hm_set(codon_map, "CCA", "P");
    hm_set(codon_map, "CCG", "P");
    hm_set(codon_map, "CAU", "H");
    hm_set(codon_map, "CAC", "H");
    hm_set(codon_map, "CAA", "Q");
    hm_set(codon_map, "CAG", "Q");
    hm_set(codon_map, "CGU", "R");
    hm_set(codon_map, "CGC", "R");
    hm_set(codon_map, "CGA", "R");
    hm_set(codon_map, "CGG", "R");
    hm_set(codon_map, "AUU", "I");
    hm_set(codon_map, "AUC", "I");
    hm_set(codon_map, "AUA", "I");
    hm_set(codon_map, "AUG", "M");
    hm_set(codon_map, "ACU", "T");
    hm_set(codon_map, "ACC", "T");
    hm_set(codon_map, "ACA", "T");
    hm_set(codon_map, "ACG", "T");
    hm_set(codon_map, "AAU", "N");
    hm_set(codon_map, "AAC", "N");
    hm_set(codon_map, "AAA", "K");
    hm_set(codon_map, "AAG", "K");
    hm_set(codon_map, "AGU", "S");
    hm_set(codon_map, "AGC", "S");
    hm_set(codon_map, "AGA", "R");
    hm_set(codon_map, "AGG", "R");
    hm_set(codon_map, "GUU", "V");
    hm_set(codon_map, "GUC", "V");
    hm_set(codon_map, "GUA", "V");
    hm_set(codon_map, "GUG", "V");
    hm_set(codon_map, "GCU", "A");
    hm_set(codon_map, "GCC", "A");
    hm_set(codon_map, "GCA", "A");
    hm_set(codon_map, "GCG", "A");
    hm_set(codon_map, "GAU", "D");
    hm_set(codon_map, "GAC", "D");
    hm_set(codon_map, "GAA", "E");
    hm_set(codon_map, "GAG", "E");
    hm_set(codon_map, "GGU", "G");
    hm_set(codon_map, "GGC", "G");
    hm_set(codon_map, "GGA", "G");
    hm_set(codon_map, "GGG", "G");
    hm_set(codon_map, "UAA", "Stop");
    hm_set(codon_map, "UGA", "Stop");
    hm_set(codon_map, "UAG", "Stop");

    return codon_map;
}

hm* protein_to_codon_map() {
    // Define codons for each amino acid
    hm* codon_map = hm_create();
    const char* F_codon[] = {"UUU", "UUC", NULL};
    const char* L_codon[] = {"UUA", "UUG", "CUU", "CUC", "CUA", "CUG", NULL};
    const char* S_codon[] = {"UCU", "UCC", "UCA", "UCG", "AGU", "AGC", NULL};
    const char* Y_codon[] = {"UAU", "UAC", NULL};
    const char* Stop_codon[] = {"UAA", "UAG", "UGA", NULL};
    const char* C_codon[] = {"UGU", "UGC", NULL};
    const char* W_codon[] = {"UGG", NULL};
    const char* P_codon[] = {"CCU", "CCC", "CCA", "CCG", NULL};
    const char* H_codon[] = {"CAU", "CAC", NULL};
    const char* Q_codon[] = {"CAA", "CAG", NULL};
    const char* R_codon[] = {"CGU", "CGC", "CGA", "CGG", "AGA", "AGG", NULL};
    const char* V_codon[] = {"GUU", "GUC", "GUA", "GUG", NULL};
    const char* A_codon[] = {"GCU", "GCC", "GCA", "GCG", NULL};
    const char* D_codon[] = {"GAU", "GAC", NULL};
    const char* E_codon[] = {"GAA", "GAG", NULL};
    const char* G_codon[] = {"GGU", "GGC", "GGA", "GGG", NULL};
    const char* I_codon[] = {"AUU", "AUC", "AUA", NULL};
    const char* M_codon[] = {"AUG", NULL};
    const char* T_codon[] = {"ACU", "ACC", "ACA", "ACG", NULL};
    const char* N_codon[] = {"AAU", "AAC", NULL};
    const char* K_codon[] = {"AAA", "AAG", NULL};

    // Insert each array into the hashmap with the corresponding amino acid as the key
    hm_set(codon_map, "F", F_codon);
    hm_set(codon_map, "L", L_codon);
    hm_set(codon_map, "S", S_codon);
    hm_set(codon_map, "Y", Y_codon);
    hm_set(codon_map, "*", Stop_codon);
    hm_set(codon_map, "C", C_codon);
    hm_set(codon_map, "W", W_codon);
    hm_set(codon_map, "P", P_codon);
    hm_set(codon_map, "H", H_codon);
    hm_set(codon_map, "Q", Q_codon);
    hm_set(codon_map, "R", R_codon);
    hm_set(codon_map, "V", V_codon);
    hm_set(codon_map, "A", A_codon);
    hm_set(codon_map, "D", D_codon);
    hm_set(codon_map, "E", E_codon);
    hm_set(codon_map, "G", G_codon);
    hm_set(codon_map, "I", I_codon);
    hm_set(codon_map, "M", M_codon);
    hm_set(codon_map, "T", T_codon);
    hm_set(codon_map, "N", N_codon);
    hm_set(codon_map, "K", K_codon);

    return codon_map;
}


int hamm(const char *seq1, const char *seq2) {
    int distance = 0;
    while (*seq1 && *seq2) {
        if (*seq1 != *seq2) {
            distance++;
        }
        seq1++;
        seq2++;
    }
    return distance;
}

// Function to reverse a string
void reverse_string(char *str) {
    int length = strlen(str);
    int start = 0;
    int end = length - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Function to generate the reverse complement of a DNA string
void reverse_complement(const char *seq, char *rev_comp) {
    int length = strlen(seq);
    for (int i = 0; i < length; i++) {
        switch (seq[length - 1 - i]) {
            case 'A': rev_comp[i] = 'T'; break;
            case 'T': rev_comp[i] = 'A'; break;
            case 'C': rev_comp[i] = 'G'; break;
            case 'G': rev_comp[i] = 'C'; break;
        }
    }
    rev_comp[length] = '\0';
}