#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

//structure to store the adjacency matrix being used by FW
typedef struct adjacencyMatrix{

  //matrix to store the adjacency values
  int **adj;

  //number of vertices
  int num_vertices;
} * AdjacencyMatrix;

//function to create an empty adjacency matrix for num_vertices vertices
//return pointer to the created empty adjacency matrix if succesfull, NULL otherwise
AdjacencyMatrix createEmptyAdjacencyMatrix(int num_vertices);

//free the memory used by the adjacency matrix
//returns 1 if succesfull, 0 otherwise
int freeAdjacencyMatrix(AdjacencyMatrix adjMat);

#endif
