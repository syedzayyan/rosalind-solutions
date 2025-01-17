#include <stdio.h>
#include <stdlib.h>  // Include stdlib.h for malloc
#include "common.h"
#include <string.h>

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