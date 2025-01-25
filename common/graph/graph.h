#include <stdlib.h>

struct edges {
    int source;      /* Source node index */
    int target;      /* Target node index */
    void* edge_data; /* Pointer to edge-specific data */
};

struct nodes {
    void* node_data;
    int d; /* number of nodes */
    int len; /* number of slots in array */
    char is_sorted;
    int* list; // Outgoing Edge Indices
};

typedef struct graph {
    int v_n; // vertex number
    int e_n; // number of edges
    struct edges* edges_list;
    struct nodes* nodes_list;
} Graph;

Graph* create_graph(int v_n);
void delete_graph(Graph* g);
int add_node_to_graph(Graph* g, void* node_data, size_t node_size);
void add_edge_to_graph(Graph* g, int u, int v, void* edge_data, size_t edge_data_size);
