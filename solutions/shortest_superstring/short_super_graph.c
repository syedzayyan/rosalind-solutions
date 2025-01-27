#include "../common.h"

// Check if seq1 and seq2 have an overlap of at least k
int has_overlap(char* seq1, char* seq2, size_t k) {
    size_t len1 = strlen(seq1);
    size_t len2 = strlen(seq2);

    if (len1 < k || len2 < k) {
        return 0;
    }

    return strncmp(seq1 + len1 - k, seq2, k) == 0;
}

// Concatenate seq1 and seq2 based on their overlap
char* merge_sequences(char* seq1, char* seq2, size_t k) {
    size_t len1 = strlen(seq1);
    size_t len2 = strlen(seq2);
    size_t overlap_len = k;

    char* merged = malloc(len1 + len2 - overlap_len + 1);
    strcpy(merged, seq1);
    strcat(merged + len1 - overlap_len, seq2);

    return merged;
}

// Find a Hamiltonian path in the graph (simple greedy implementation)
int find_hamiltonian_path(Graph* graph, int* path) {
    int visited[graph->v_n];
    memset(visited, 0, sizeof(visited));

    int current = 0;  // Start with the first node
    int path_idx = 0;
    path[path_idx++] = current;
    visited[current] = 1;

    while (path_idx < graph->v_n) {
        int found_next = 0;
        for (int i = 0; i < graph->nodes_list[current].d; i++) {
            int next = graph->nodes_list[current].list[i];
            if (!visited[next]) {
                path[path_idx++] = next;
                visited[next] = 1;
                current = next;
                found_next = 1;
                break;
            }
        }
        if (!found_next) {
            return 0;  // No Hamiltonian path found
        }
    }
    return 1;  // Successfully found a Hamiltonian path
}

int main() {
    // Load FASTA sequences
    hm* fasta_genes = fasta_file_reader("./rosalind_grph.txt");
    if (!fasta_genes) {
        fprintf(stderr, "Error reading FASTA file.\n");
        return 1;
    }

    Graph* graph = create_graph(2);  // Initialize graph

    // Add nodes and edges based on overlaps
    hmi iter1 = ht_iter(fasta_genes);
    while (hm_nxt(&iter1)) {
        char* seq1 = (char*)iter1.value;

        hmi iter2 = ht_iter(fasta_genes);
        while (hm_nxt(&iter2)) {
            char* seq2 = (char*)iter2.value;
            if (seq1 != seq2 && has_overlap(seq1, seq2, strlen(seq1) / 2)) {
                int u = add_node_to_graph(graph, seq1, strlen(seq1));
                int v = add_node_to_graph(graph, seq2, strlen(seq2));
                add_edge_to_graph(graph, u, v, NULL, 0);
            }
        }
    }

    // Find Hamiltonian path
    int* path = malloc(graph->v_n * sizeof(int));
    if (!find_hamiltonian_path(graph, path)) {
        fprintf(stderr, "No Hamiltonian path found.\n");
        delete_graph(graph);
        free(path);
        return 1;
    }

    // Reconstruct superstring
    char* superstring = strdup((char*)graph->nodes_list[path[0]].node_data);
    for (int i = 1; i < graph->v_n; i++) {
        char* next_seq = (char*)graph->nodes_list[path[i]].node_data;
        size_t overlap = strlen(next_seq) / 2;  // Assume k = len(seq) / 2
        char* merged = merge_sequences(superstring, next_seq, overlap);
        free(superstring);
        superstring = merged;
    }

    printf("Shortest Superstring: %s\n", superstring);

    // Cleanup
    free(superstring);
    free(path);
    delete_graph(graph);

    return 0;
}
