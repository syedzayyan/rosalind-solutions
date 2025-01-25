// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Graphs.html
#include "graph.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Graph* create_graph(int v_n) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    assert(g);

    g->v_n = v_n;
    g->e_n = 0;

    // Allocate memory for the nodes_list (array of nodes)
    g->nodes_list = malloc(sizeof(struct nodes) * v_n);
    assert(g->nodes_list);

    // Initialize the nodes_list
    for (int i = 0; i < v_n; i++) {
        // Initialize each node's data
        g->nodes_list[i].node_data = NULL; // Initialize node data to NULL
        g->nodes_list[i].d = 0;            // No outgoing edges yet
        g->nodes_list[i].len = 0;           // No space allocated for outgoing edges
        g->nodes_list[i].is_sorted = 1;     // Set the sorted flag (can be updated later)
        g->nodes_list[i].list = NULL;       // Initialize outgoing edge list to NULL
    }

    // Initially, no edges are allocated.
    g->edges_list = NULL;

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