// function declarations
#include "./common/hashmap/hm.h"
#include "./common/hashmap/hm_v3.h"
#include "./common/set/btree.h"
#include "./common/graph/graph.h"
#include "./common/fasta_file_reader/ffr.h"

#include <stdio.h>

char* file_to_char(const char *filename);
hm* init_codon_map();
hm* protein_to_codon_map();
char** read_fasta_to_array(const char* filename, int* num_sequences);

int hamm(const char *seq1, const char *seq2);
void reverse_string(char *str);
void reverse_complement(const char *seq, char *rev_comp);