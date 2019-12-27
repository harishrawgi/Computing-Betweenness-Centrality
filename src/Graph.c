#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "GraphHelper.h"

//function to read and create a graph from the file
//function expects a file name  as input (with fully qualified path) and the number of vertices
//ch is for whether the file has weigths or not
//returns pointer to the created graph if succesfull, NULL otherwise
Graph populateGraphFromFile(char* file, int n, int ch){

  //input validation
  if(!file || n<=0 || (ch!=1 && ch!=0)){
    printf("\nError in function readGraphFromFile(): Invalid input parameters.\n");
    return NULL;
  }

  //create an empty graph with n vertices
  Graph graph = createEmptyGraph(n);
  if(!graph){
    printf("\nError in function readGraphFromFile(): Unable to create an empty graph.\n");
    return NULL;
  }

  //open the file
  FILE* fp;
  fp = fopen(file, "r");
  if(!fp){
    printf("\nError in function readGraphFromFile(): Unable to open file (%s)\n", file);
    return NULL;
  }

  //variables used an intermediates in reading the file
  int read_successfull = 0;
  int src, dest, wt;

  // 0 means it is an ER graph with vertex id starting from 0 and no weight
  if(ch == 0){
    while(1){
      int read_result = fscanf(fp, "%d %d\n", &src, &dest);
      if(read_result == 2){
        if(!addEdgeToGraph(graph, src, dest)){
          printf("\nError in function readGraphFromFile(): Failed to add an edge (%d, %d)\n", src, dest);
          return NULL;
        }
      }
      else if(read_result == EOF){
        printf("\nMessage from readGraphFromFile(): Graph read successfully from file.\n");
        return graph;
      }
      else{
        printf("\nError in function readGraphFromFile(): Invalid input in file.");
        return NULL;
      }
    }
  }
  // 1 means it is the sample Cornell graph with vertex id starting from 1 and edge weights
  //the weights will be ignored
  else if(ch == 1){
    while(1){
      int read_result = fscanf(fp, "%d %d %d\n", &src, &dest, &wt);
      if(read_result == 3){
        if(!addEdgeToGraph(graph, src-1, dest-1)){
          printf("\nError in function readGraphFromFile(): Failed to add an edge (%d, %d)\n", src, dest);
          return NULL;
        }
      }
      else if(read_result == EOF){
        printf("\nMessage from readGraphFromFile(): Graph read successfully from file.\n");
        return graph;
      }
      else{
        printf("\nError in function readGraphFromFile(): Invalid input in file.");
        return NULL;
      }
    }
  }

}

//function to calculate Betweenness Centrality using BFS for each vertex of the supplied graph
//returns 1 if succesfull, 0 otherwise
int calculateBCBFS(Graph graph){

  //validating inputs
  if(!graph){
    printf("\nError in calculateBCBFS(): NULL passed as an input parameter.\n");
    return 0;
  }

  // each call in the loop calulates the BC values for all vertex with "i" as the src vertex
  // meaning all pairs (i,j) where "i" is the src vertex,
  // for all such pairs the BC values for each vertex is calculated and added up
  for(int i=0; i<graph->num_vertices; i++){
    // the function bfsBC() calculates and adds BC values for each vertex for all pairs (i,j)
    if(!bfsBC(graph, i)){
      printf("\nError in calculateBCBFS(): Error while running BFS BC procedure for vertex with id (%d).\n", i);
      return 0;
    }
  }

  //we must divide the BC values by 2
  // since the previous loop would have counted the BC values twice for each pair, since undirected graph
  for(int i=0; i<graph->num_vertices; i++){
    graph->bc[i] = graph->bc[i]/2;
  }

  //return 1 if succesfull
  return 1;
}

//function to calculate Betweenness Centrality using FW for each vertex of the supplied graph
//returns 1 if succesfull, 0 otherwise
int calculateBCFW(Graph graph){

  //validating inputs
  if(!graph){
    printf("\nError in calculateBCFW(): NULL passed as an input parameter.\n");
    return 0;
  }

  //create an adjacency matrix for the input graph
  AdjacencyMatrix adjMat = createAdjMat(graph);
  if(!adjMat){
    printf("\nError in calculateBCFW(): Unable to create an adjacency matrix for the graph.\n");
    return 0;
  }

  //call the function which calculates the actual BC values for the graph
  if(!calculateBCFW_helper(adjMat, graph)){
    printf("\nError in calculateBCFW(): Unable to calculate BC values using Floyd-Warshall.\n");
    return 0;
  }

  //free-up the memory used by the adjacency matrix
  if(!freeAdjacencyMatrix(adjMat)){
    printf("\nError in calculateBCFW(): Unable to free the adjacency matrix.\n");
    return 0;
  }

  //return 1 if succesfull
  return 1;
}

//function to print the graph in an adjacency list form
int printGraph(Graph graph){

  //validating inputs
  if(!graph){
    printf("\nError in printGraph(): NULL passed in as parameter.\n");
    return 0;
  }

  //print the graph in the form of an adjacency list
  for(int i=0; i<graph->num_vertices; i++){
    struct adjList adj = graph->vertexArray[i];
    AdjListNode curr = adj.head;
    while(curr){
      printf("%d->%d \t", i, curr->vertexId);
      curr = curr->next;
    }
    printf("\n");
  }

  return 1;
}

//function to normalize the BC values
int normalizeBCvals(float *bc_vals, int n){

  if(!bc_vals || n<=0){
    printf("\nError in normalizeBCvals(): Invalid input parameters.\n");
    return 0;
  }

  //calculating the min, max BC values
  float min_bc=0, max_bc=0;
  for(int i=0; i<n; i++){
    if(min_bc > bc_vals[i]) min_bc = bc_vals[i];
    if(max_bc < bc_vals[i]) max_bc = bc_vals[i];
  }

  //denominators used in normalization
  float normalizer_bfs = max_bc-min_bc;

  //normalizing the values individually
  float normalized_bc;
  for(int i=0; i<n; i++){
    normalized_bc = (bc_vals[i] - min_bc)/normalizer_bfs;
    bc_vals[i] = normalized_bc;
  }

  return 1;
}

//function to free the memory used by graph
int freeGraph(Graph graph){

  //validating inputs
  if(!graph){
    printf("\nError in freeGraph(): NULL passed in as input parameter.");
    return 0;
  }

  //there is no vertex array
  if(!graph->vertexArray){
    free(graph);
    graph = NULL;
    return 1;
  }

  //free the vertex array
  if(!freeAdjList(graph->vertexArray)){
    printf("\nError in freeGraph(): Unable to free the vertex array.");
    return 0;
  }

  // free the pointer to the graph structure
  free(graph);
  graph = NULL;
  return 1;
}
