#include "../common.h"

int has_overlap(char* seq1, char* seq2, size_t k) {
    size_t len1 = strlen(seq1);
    size_t len2 = strlen(seq2);

    // Ensure sequences are long enough for k comparison
    if (len1 < k || len2 < k) {
        return 0;
    }

    // Compare the last k characters of seq1 with the first k characters of seq2
    return strncmp(seq1 + len1 - k, seq2, k);
}

int main(){
    hm* fasta_genes = fasta_file_reader("./rosalind_grph.txt");
    hmi fasta_gene_it = ht_iter(fasta_genes);

    // Extract labels and sequences from the hashmap
    while (hm_nxt(&fasta_gene_it)) {
        hmi fasta_gene_it_second = ht_iter(fasta_genes);
        while (hm_nxt(&fasta_gene_it_second)) {
            if (
                strcmp(fasta_gene_it.key, fasta_gene_it_second.key) != 0 &&
                has_overlap((char*)fasta_gene_it.value, (char*)fasta_gene_it_second.value, 3)  == 0
                ) {
                printf("%s %s\n", fasta_gene_it.key, fasta_gene_it_second.key);
            }
        }
    }    
    return 0;
}