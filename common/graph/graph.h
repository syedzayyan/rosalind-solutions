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

Graph* create_graph();
void delete_graph(Graph* g);
int add_node_to_graph(Graph* g, void* node_data, size_t node_size);
void add_edge_to_graph(Graph* g, int u, int v, void* edge_data, size_t edge_data_size);

int** get_adjacency_matrix(Graph* g);
void print_adjacency_matrix(Graph* g);

void delete_edge_from_graph(Graph* g, int edge_id);
void delete_node_from_graph(Graph* g, int node_id);

// #define SEARCH_INFO_NULL (-1)

// struct search_info {
//     Graph graph;
//     int reached;        /* count of reached nodes */
//     int *preorder;      /* list of nodes in order first reached */
//     int *time;          /* time[i] == position of node i in preorder list */
//     int *parent;        /* parent in DFS or BFS forest */
//     int *depth;         /* distance from root */
// };

// /* allocate and initialize search results structure */
// /* you need to do this before passing it to dfs or bfs */
// struct search_info *search_info_create(Graph g);

// /* free search_info data---does NOT free graph pointer */
// void search_info_destroy(struct search_info *);

// /* perform depth-first search starting at root, updating results */
// void dfs(struct search_info *results, int root);

// /* perform breadth-first search starting at root, updating results */
// void bfs(struct search_info *results, int root);