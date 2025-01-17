#include <stdio.h>
#include "../common.h"
#include <stdlib.h>  // Include stdlib.h for malloc
#include <string.h>

int main(){
    hm* codon_map = init_codon_map();
    char* rna = file_to_char("rosalind_prot.txt");

    for (int i = 0; i + 2 < strlen(rna); i += 3) {
        char codon[4] = {rna[i], rna[i+1], rna[i+2], '\0'};
        char* amino = hm_get(codon_map, codon);
        printf("%s", amino);
    }
    printf("\n");

    return 0;
}