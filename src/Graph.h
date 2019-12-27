#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "AdjList.h"

//structure for the graph data structure
typedef struct graph {

  //number of vertices in the graph
  int num_vertices;

  //number of edges in the graph
  int num_edges;

  //array of adjacency lists for each vertex
  AdjList vertexArray;

  //array of real values, which stores the Betweenness Centrality of each vertex
  float* bc;

} * Graph;

//function to read and create a graph from the file
//function expects a file name  as input (with fully qualified path) and the number of vertices
//ch is for whether the file has weigths or not
//returns pointer to the created graph if succesfull, NULL otherwise
Graph populateGraphFromFile(char* file, int n, int ch);

//function to calculate Betweenness Centrality using BFS for each vertex of the supplied graph
//returns 1 if succesfull, 0 otherwise
int calculateBCBFS(Graph graph);

//function to calculate Betweenness Centrality using FW for each vertex of the supplied graph
//returns 1 if succesfull, 0 otherwise
int calculateBCFW(Graph graph);

//function to print the graph in an adjacency list form
int printGraph(Graph graph);

//function to normalize the BC values
int normalizeBCvals(float *bc_vals, int n);

//function to free the memory used by graph
int freeGraph(Graph graph);

#endif
