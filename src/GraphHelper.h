#ifndef GRAPHHELPER_H
#define GRAPHHELPER_H

#include "Graph.h"
#include "PredecessorList.h"
#include "AdjacencyMatrix.h"

#define INFINITY 999999

//function to create an empty graph (no edges) with supplied number of vertices
//returns pointer to the graph if succesfull, NULL otherwise
Graph createEmptyGraph(int num_vertices);

//function to add a new edge to the graph with source as src and destination as dest
//adds two edges as the graph is undirected
int addEdgeToGraph(Graph graph, int src, int dest);

//function which performs BC computation for all vertices corresponding to startVertexId as the source
//BC values which are computed are added to the graph which is taken as input
//returns 1 if succesfull, 0 otherwise
int bfsBC(Graph graph, int startVertexId);

//function to create an adjacency matrix from the given undirected graph
AdjacencyMatrix createAdjMat(Graph graph);

//helper function to calculate the BC values for the graph from the adjacency matrix using FW
int calculateBCFW_helper(AdjacencyMatrix adjMat, Graph graph);


#endif
