// function declarations
#include "./common/hashmap/hm.h"
#include "./common/set/btree.h"
#include "./common/graph/graph.h"
#include "./common/fasta_file_reader/ffr.h"

#include <stdio.h>

char* file_to_char(const char *filename);
hm* init_codon_map();