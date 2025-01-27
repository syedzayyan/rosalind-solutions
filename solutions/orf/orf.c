#include <stdio.h>
#include "../common.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void print_protein_from_orfs(char *tmp_content, hm *codon_map)
{
    if (strlen(tmp_content) < 3)
        return;
    bool in_orf = false;
    char *start_pos = tmp_content;  // Keep track of where we started

    char tempCodon[strlen(tmp_content)];
    int tempCodonIdx;
    BTNode* root = NULL;
    size_t sizeStr = strlen(tmp_content);

    while (*tmp_content)
    {
        char codon[4];
        strncpy(codon, tmp_content, 3);
        codon[3] = '\0';

        // Check for a start codon
        if (!in_orf && strncmp(codon, "AUG", 3) == 0)
        {
            in_orf = true; // Start translating
            start_pos = tmp_content;
            // tmp_content += 3; // Move to next codon
            // continue;
        }

        // If in an ORF, process the codon
        if (in_orf)
        {
            // Check for stop codons
            if (strncmp(codon, "UAG", 3) == 0 || strncmp(codon, "UGA", 3) == 0 || strncmp(codon, "UAA", 3) == 0)
            {
                // End of current ORF
                in_orf = false; // Reset ORF state
                tmp_content = start_pos + 1;
                tempCodon[tempCodonIdx] = '\0';
                root = add(root, tempCodon, strlen(tempCodon) + 1, compareString);
                tempCodonIdx = 0;
                continue;
            }

            // Translate codon to amino acid
            char *amino = hm_get(codon_map, codon);
            if (amino)
            {
                tempCodon[tempCodonIdx] = *amino;
                tempCodonIdx++;
            }
            else
            {
                fprintf(stderr, "Unknown codon: %s\n", codon);
            }

            tmp_content += 3; // Move to next codon
            continue;
        }

        // Move to the next nucleotide if not in an ORF
        tmp_content++;
    }
    inOrderTraversalPrintString(root);
    printf("\n");
}


void reverse_complement(char *dna, char *rev_comp) {
    size_t len = strlen(dna);
    for (size_t i = 0; i < len; i++) {
        switch (dna[len - i - 1]) {
            case 'A': rev_comp[i] = 'T'; break;
            case 'T': rev_comp[i] = 'A'; break;
            case 'C': rev_comp[i] = 'G'; break;
            case 'G': rev_comp[i] = 'C'; break;
        }
    }
    rev_comp[len] = '\0';
}

void dna_to_rna(char *dna) {
    for (size_t i = 0; dna[i] != '\0'; i++) {
        if (dna[i] == 'T') {
            dna[i] = 'U';
        }
    }
}

int main()
{
    char *content = file_to_char("ss.txt");
    if (content == NULL)
    {
        return 1; // Exit if file reading failed
    }

    hm *codon_map = init_codon_map();
    char *normal_strand = content; // Use tmp_content for iteration to preserve original content

    while (normal_strand)
    {
        char *init_fasta = memchr(normal_strand, '>', strlen(normal_strand));
        if (init_fasta == NULL)
        {
            break;
        }
        normal_strand = init_fasta + 1;

        char *init_seq = memchr(normal_strand, '\n', strlen(normal_strand));
        if (init_seq == NULL)
        {
            break;
        }
        char identifier[init_seq - normal_strand + 1];
        strncpy(identifier, normal_strand, init_seq - normal_strand);
        identifier[init_seq - normal_strand] = '\0';
        normal_strand = init_seq + 1;
    }
    
    char *complementary = (char *)malloc(strlen(normal_strand) + 1);
    if (complementary == NULL) {
        perror("Memory allocation error");
        return 1;
    }
    
    reverse_complement(normal_strand, complementary);

    dna_to_rna(normal_strand);
    dna_to_rna(complementary);

    print_protein_from_orfs(normal_strand, codon_map);
    print_protein_from_orfs(complementary, codon_map);

    free(complementary);
    free(content); // Free the allocated content after usage
    return 0;
}
