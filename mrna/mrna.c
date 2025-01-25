#include "../common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // For uint64_t if needed

#define MODULO 1000000

int count_codons(void* data) {
    const char** codons = (const char**)data;
    int count = 0;
    // Loop through the codons array until NULL is encountered
    for (int i = 0; codons[i] != NULL; i++) {
        count += 1;
    }
    return count;
}

int main() {
    hm *protein_codon = hm_create(); // HashMap to store codon mappings

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

    // Insert codons into the hashmap
    hm_set(protein_codon, "F", F_codon);
    hm_set(protein_codon, "L", L_codon);
    hm_set(protein_codon, "S", S_codon);
    hm_set(protein_codon, "Y", Y_codon);
    hm_set(protein_codon, "*", Stop_codon);
    hm_set(protein_codon, "C", C_codon);
    hm_set(protein_codon, "W", W_codon);
    hm_set(protein_codon, "P", P_codon);
    hm_set(protein_codon, "H", H_codon);
    hm_set(protein_codon, "Q", Q_codon);
    hm_set(protein_codon, "R", R_codon);
    hm_set(protein_codon, "V", V_codon);
    hm_set(protein_codon, "A", A_codon);
    hm_set(protein_codon, "D", D_codon);
    hm_set(protein_codon, "E", E_codon);
    hm_set(protein_codon, "G", G_codon);
    hm_set(protein_codon, "I", I_codon);
    hm_set(protein_codon, "M", M_codon);
    hm_set(protein_codon, "T", T_codon);
    hm_set(protein_codon, "N", N_codon);
    hm_set(protein_codon, "K", K_codon);

    char *content = file_to_char("rosalind_mrna.txt");
    char *ptr = content;
    long int number = 1;

    while (*ptr) {
        char key[2] = {*ptr, '\0'};
        void* data = hm_get(protein_codon, key);
        
        if (data == NULL) {
            printf("Error: Invalid amino acid '%c'\n", *ptr);
            free(content);
            ht_delete(protein_codon);
            return 1;
        }

        long int codon_count = count_codons(data);
        number = (number * codon_count) % MODULO; // Apply modulo to prevent overflow
        ptr++;
    }

    number = (number * 3) % MODULO; // Account for stop codons
    printf("%li\n", number);

    free(content);
    ht_delete(protein_codon);
    return 0;
}
