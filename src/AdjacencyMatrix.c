#include <stdio.h>
#include <stdlib.h>
#include "AdjacencyMatrix.h"

//function to create an empty adjacency matrix for num_vertices vertices
//return pointer to the created empty adjacency matrix if succesfull, NULL otherwise
AdjacencyMatrix createEmptyAdjacencyMatrix(int num_vertices){

  //validating inputs
  if(num_vertices <= 0){
    printf("\nError in function createEmptyAdjacencyMatrix(): number of vertices passed as non-positive.\n");
    return NULL;
  }

  //allocate memory for the adjacencyMatrix structure
  AdjacencyMatrix adjMat = (AdjacencyMatrix) malloc(sizeof(struct adjacencyMatrix));
  if(!adjMat){
    printf("\nError in function createEmptyAdjacencyMatrix(): failed to allocate memory for the graph.\n");
    return NULL;
  }

  //set the number of vertices
  adjMat->num_vertices = num_vertices;

  //allocate memory for the integer matrix
  adjMat->adj = (int**) malloc(num_vertices*sizeof(int*));
  if(!adjMat->adj){
    printf("\nError in function createEmptyAdjacencyMatrix(): failed to allocate memory for the matrix.\n");
    return NULL;
  }

  //allocate memory for the individual rows of the matrix
  for(int i=0; i<num_vertices; i++){
    adjMat->adj[i] = (int *) malloc(num_vertices*sizeof(int));
    if(!adjMat->adj[i]){
      printf("\nError in function createEmptyAdjacencyMatrix(): failed to allocate memory for a row in matrix.\n");
      return NULL;
    }
  }

  //return the created adjacency matrix
  return adjMat;
}

//free the memory used by the adjacency matrix
//returns 1 if succesfull, 0 otherwise
int freeAdjacencyMatrix(AdjacencyMatrix adjMat){

  //validating inputs
  if(!adjMat){
    printf("\nError in function freeAdjacencyMatrix(): NULL passed in as input parameter.\n");
    return 0;
  }

  //free the individual rows of the matrix
  if(adjMat->adj){
    for(int i=0; i<adjMat->num_vertices; i++){
      if(adjMat->adj[i]){
        free(adjMat->adj[i]);
        adjMat->adj[i] = NULL;
      }
    }
    //free the matrix pointer
    free(adjMat->adj);
    adjMat->adj = NULL;
  }

  //free the pointer to the structure
  free(adjMat);
  adjMat = NULL;

  //retrun 1 if succesfull
  return 1;
}
