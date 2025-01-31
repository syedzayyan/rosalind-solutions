// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Graphs.html
#include "graph.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Graph* create_graph() {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    assert(g);

    g->v_n = 0;
    g->e_n = 0;

    // Allocate memory for the nodes_list (array of nodes)
    g->nodes_list = malloc(sizeof(struct nodes));
    assert(g->nodes_list);
    // Initially, no edges are allocated.
    g->edges_list = malloc(sizeof(struct edges));
    return g;
}

void delete_graph(Graph* g) {
    // Free each node's outgoing edge list (if allocated)
    for (int i = 0; i < g->v_n; i++) {
        if (g->nodes_list[i].list != NULL) {
            free(g->nodes_list[i].list);  // Free the outgoing edges list
        }
        if (g->nodes_list[i].node_data != NULL) {
            free(g->nodes_list[i].node_data);
        }
    }

    // Free the nodes_list array (this holds all nodes)
    free(g->nodes_list);

    // Free each edge's data (if any) and the edges_list array
    if (g->edges_list != NULL) {
        free(g->edges_list);  // Free the edges list
    }

    // Finally, free the graph object itself
    free(g);
}

int add_node_to_graph(Graph* g, void* node_data, size_t node_size){
    int curr_node_idx = g->v_n;
    g->nodes_list = realloc(g->nodes_list, sizeof(struct nodes) * (curr_node_idx + 1));
    assert(g->nodes_list);

    g->nodes_list[curr_node_idx].node_data = malloc(node_size);
    assert(g->nodes_list[curr_node_idx].node_data);
    memcpy(g->nodes_list[curr_node_idx].node_data, node_data, node_size);

    // Initialize other fields for the new node
    g->nodes_list[curr_node_idx].d = 0;        // No outgoing edges yet
    g->nodes_list[curr_node_idx].len = 0;       // No space allocated for outgoing edges
    g->nodes_list[curr_node_idx].is_sorted = 1; // Set the sorted flag (this can be updated later)
    g->nodes_list[curr_node_idx].list = NULL;   // Outgoing edge list is NULL initially

    g->v_n++;
    return curr_node_idx;
}

void add_edge_to_graph(Graph* g, int u, int v, void* edge_data, size_t edge_data_size){
    // Just check input stuff isn't garbage
    assert(u >= 0 && u < g->v_n);
    assert(v >= 0 && v < g->v_n);

    int curr_edge_idx = g->e_n;

    // Add the edge data to the edge list (reallocating if necessary)
    g->edges_list = realloc(g->edges_list, sizeof(struct edges) * (g->e_n + 1));
    assert(g->edges_list);  // Ensure memory allocation was successful

    g->edges_list[curr_edge_idx].source = u;
    g->edges_list[curr_edge_idx].target = v;


    if (edge_data_size > 0) {
        g->edges_list[curr_edge_idx].edge_data = malloc(edge_data_size);
        assert(g->edges_list[curr_edge_idx].edge_data); // Check memory wasn't garbagely allocated
        memcpy(g->edges_list[curr_edge_idx].edge_data, edge_data, edge_data_size);
    } else {
        g->edges_list[curr_edge_idx].edge_data = NULL;
    }


    if (g->nodes_list[u].d >= g->nodes_list[u].len) { // If we have no memory left in the node array, we need to reallocate
        if (g->nodes_list[u].len > 0) {
            g->nodes_list[u].len *= 2; 
        } else {
            g->nodes_list[u].len = 1;
        }
        g->nodes_list[u].list = realloc(g->nodes_list[u].list, sizeof(int) * g->nodes_list[u].len);
        assert(g->nodes_list[u].list);  // Ensure memory allocation wasn't garbage
    }

    g->nodes_list[u].list[g->nodes_list[u].d++] = v;
    g->nodes_list[u].is_sorted = 0;
    // Increment the edge count
    g->e_n++;
}

void delete_edge_from_graph(Graph* g, int edge_id){
    if (!g || edge_id < 0 || edge_id >= g->e_n) return;

    // Free edge data
    if (g->edges_list[edge_id].edge_data) {
        if (g->edges_list[edge_id].edge_data) free(g->edges_list[edge_id].edge_data);
        g->edges_list[edge_id].edge_data = NULL;
    }

    // Shift edges left to fill the gap
    for (int i = edge_id; i < g->e_n - 1; i++) {
        g->edges_list[i] = g->edges_list[i + 1];
    }

    // Decrement edge count
    g->e_n--;

    g->edges_list = realloc(g->edges_list, g->e_n * sizeof(struct edges));
}


void delete_node_from_graph(Graph* g, int node_id){
    if (!g || node_id < 0 || node_id >= g->v_n) return;
    // Step 1 Look at Edge Lists 

    // Step 1: Delete edges that contain the node (iterate **backwards**)
    for (int i = g->e_n - 1; i >= 0; i--) {
        if (g->edges_list[i].source == node_id || g->edges_list[i].target == node_id) {
            delete_edge_from_graph(g, i);
        }
    }

    if (g->nodes_list[node_id].node_data) free(g->nodes_list[node_id].node_data); // Free node data if needed
    if (g->nodes_list[node_id].list) free(g->nodes_list[node_id].list);

    for (int i = node_id; i < g->v_n - 1; i++) {
        g->nodes_list[i] = g->nodes_list[i + 1]; // Shift nodes
    }


    for (int i = 0; i < g->v_n - 1; i++) { // New size is g->v_n - 1
        if (g->nodes_list[i].list) {
            for (int j = 0; j < g->nodes_list[i].d; j++) {
                if (g->nodes_list[i].list[j] == node_id) {
                    // Remove the reference
                    for (int k = j; k < g->nodes_list[i].d - 1; k++) {
                        g->nodes_list[i].list[k] = g->nodes_list[i].list[k + 1];
                    }
                    g->nodes_list[i].d--;
                }
                // Adjust indices for nodes that were shifted
                else if (g->nodes_list[i].list[j] > node_id) {
                    g->nodes_list[i].list[j]--;
                }
            }
        }
    }


    g->v_n--;
}   


// Things to do with Graph
int** get_adjacency_matrix(Graph* g){
    if (!g || g->v_n <= 0) return NULL; // Make sure garbage is not given

    int** matrix_2d = (int**)malloc(g->v_n * sizeof(int));
    if (matrix_2d == NULL) return NULL; // Make sure thing got actually assigned

    for (int i = 0; i < g->v_n; i++){
        matrix_2d[i] = (int*)calloc(g->v_n, sizeof(int)); // Assigning the rowss
    }

    for (int i = 0; i < g->v_n; i++){
        struct nodes curr_node = g->nodes_list[i];
        for (int j = 0; j < curr_node.d; j++) { // so basically getting degree of each node
            int target = curr_node.list[j];
            matrix_2d[i][target] = 1; // Set edge presence
        }
    }
    return matrix_2d;
}

/* Helper function to print adjacency matrix */
void print_adjacency_matrix(Graph* g) {
    int** matrix = get_adjacency_matrix(g);
    int v_n = g->v_n;
    for (int i = 0; i < v_n; i++) {
        for (int j = 0; j < v_n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// static int *create_empty_array(int n)
// {
//     int *a;
//     int i;

//     a = malloc(sizeof(*a) * n);
//     assert(a);

//     for(i = 0; i < n; i++) {
//         a[i] = SEARCH_INFO_NULL;
//     }

//     return a;
// }

// /* allocate and initialize search results structure */
// /* you need to do this before passing it to dfs or bfs */
// struct search_info *
// search_info_create(Graph g)
// {
//     struct search_info *s;
//     int n;

//     s = malloc(sizeof(*s));
//     assert(s);

//     s->graph = g;
//     s->reached = 0;

//     n = graph_vertex_count(g);

//     s->preorder = create_empty_array(n);
//     s->time = create_empty_array(n);
//     s->parent = create_empty_array(n);
//     s->depth = create_empty_array(n);

//     return s;
// } 

// /* free search_info data---does NOT free graph pointer */
// void
// search_info_destroy(struct search_info *s)
// {
//     free(s->depth);
//     free(s->parent);
//     free(s->time);
//     free(s->preorder);
//     free(s);
// }

// /* used inside search routines */
// struct edge {
//     int u;          /* source */
//     int v;          /* sink */
// };

// /* stack/queue */
// struct queue {
//     struct edge *e;
//     int bottom;
//     int top;
// };

// static void
// push_edge(Graph g, int u, int v, void *data)
// {
//     struct queue *q;

//     q = data;

//     assert(q->top < graph_edge_count(g) + 1);

//     q->e[q->top].u = u;
//     q->e[q->top].v = v;
//     q->top++;
// }

// /* this rather horrible function implements dfs if use_queue == 0 */
// /* and bfs if use_queue == 1 */
// static void
// generic_search(struct search_info *r, int root, int use_queue)
// {
//     /* queue/stack */
//     struct queue q;

//     /* edge we are working on */
//     struct edge cur;

//     /* start with empty q */
//     /* we need one space per edge */
//     /* plus one for the fake (root, root) edge */
//      q.e = malloc(sizeof(*q.e) * (graph_edge_count(r->graph) + 1));
//      assert(q.e);
 
//      q.bottom = q.top = 0;
 
//      /* push the root */
//      push_edge(r->graph, root, root, &q);
 
//      /* while q.e not empty */
//      while(q.bottom < q.top) {
//          if(use_queue) {
//              cur = q.e[q.bottom++];
//          } else {
//              cur = q.e[--q.top];
//          }
 
//          /* did we visit sink already? */
//          if(r->parent[cur.v] != SEARCH_INFO_NULL) continue;
 
//          /* no */
//          assert(r->reached < graph_vertex_count(r->graph));
//          r->parent[cur.v] = cur.u;
//          r->time[cur.v] = r->reached;
//          r->preorder[r->reached++] = cur.v;
//          if(cur.u == cur.v) {
//              /* we could avoid this if we were certain SEARCH_INFO_NULL */
//              /* would never be anything but -1 */
//              r->depth[cur.v] = 0;
//          } else {
//              r->depth[cur.v] = r->depth[cur.u] + 1;
//          }
 
//          /* push all outgoing edges */
//          graph_foreach(r->graph, cur.v, push_edge, &q);
//      }
 
//      free(q.e);
//  }
 
//  void
//  dfs(struct search_info *results, int root)
//  {
//      generic_search(results, root, 0);
//  }
 
//  void
//  bfs(struct search_info *results, int root)
//  {
//      generic_search(results, root, 1);
//  }