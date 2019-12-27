#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include "Graph.h"

#define N 100

int main(){

  // Input the number of vertices in the graph which is to be processed
  int num_vertices;
  printf("\nEnter the number of vertices in the graph for which BC values are to be caluclated: \t");
  if(1 != scanf("%d", &num_vertices)){
    printf("\nError in main(): Unable to read number of vertices.\n");
    exit(0);
  }
  if(num_vertices <= 0){
    printf("\nError in main(): Invalid input, number of vertices should be positive.\n");
    exit(0);
  }

  // Input the choice for what kind of graph is to be processed
  // 0 represents random graph (generated using Erdos-Renyi)
  // this graph has vertex id starting from 0 and no weights
  // any other value represents the Cornell sample graph.
  // this graph has vertex id starting from 1 and also has weights which will be ignored
  int ch;
  printf("\nEnter the choice of graph (ER Graph = 0, Cornell Graph = 1): \t");
  if(1 != scanf("%d", &ch)){
    printf("\nError in main(): Unable to read the choice of graph.\n");
    exit(0);
  }
  if(ch != 0 && ch != 1){
    printf("\nError in main(): Invalid input, enter either 0 or 1.\n");
    exit(0);
  }

  // Input to decide whether to print normalized BC values or not
  int toNormalize;
  printf("\nEnter whether BC values should be normalized (Normalize = 1, Don't Normalize = 0): \t");
  if(1 != scanf("%d", &toNormalize)){
    printf("\nError in main(): Unable to read the choice of normalization.\n");
    exit(0);
  }
  if(toNormalize != 0 && toNormalize != 1){
    printf("\nError in main(): Invalid input, enter either 0 or 1.\n");
    exit(0);
  }

  //enter the filename with fully qualified path where the graph text file is stored
  char file[N];  // example = "/home/hari/DSA_Assignments/Assignment3/BetweennessCentrality/graphs/ERGraph_5000__10_3.txt"
  printf("\nEnter the name of the file storing the graph (with fully qualified path): \t");
  if(0 == scanf("%s", file)){
    printf("\nError in main(): Unable to read file name.\n");
    exit(0);
  }

  // clock_t variables for storing timestamps to calculate execution times of calculating BC values
  clock_t start_time_bfs, end_time_bfs, start_time_fw, end_time_fw;
  // variables to store the time taken by both implementations
  double time_taken_bfs, time_taken_fw;

  // reading the graph from the file
  Graph graph = populateGraphFromFile(file, num_vertices, ch);
  if(!graph){
    printf("\nError in main(): Unable to populate the graph from the file(%s).\n", file);
    exit(0);
  }

  // start of the BFS implementation to calculate BC values
  start_time_bfs = clock();

  if(!calculateBCBFS(graph)){
    printf("\nError in main(): Failed to calculate BC values for the graph.\n");
    exit(0);
  }

  end_time_bfs = clock();
  // end of the BFS implementation to calculate BC values

  //creating an array to store BC values calculated using BFS
  float *bc_bfs = (float *) malloc(num_vertices*sizeof(float));
  if(!bc_bfs){
    printf("\nError in main(): Unable to allocate memory for storing BC values of BFS.\n");
    exit(0);
  }
  for(int i=0; i<graph->num_vertices; i++){
    bc_bfs[i] = graph->bc[i];

    //resetting the BC values in the graph because we'll again use this for FW implementation
    graph->bc[i]=0;
  }

  // start of FW implementation for calculating BC values
  start_time_fw = clock();

  if(!calculateBCFW(graph)){
    printf("\nError in main(): Failed to calculate BC values for the graph.\n");
    exit(0);
  }

  end_time_fw = clock();
  // end of FW implementation for calculating BC values

  //storing the BC values calculated using FW implementation
  float *bc_fw = (float *) malloc(num_vertices*sizeof(float));
  if(!bc_fw){
    printf("\nError in main(): Unable to allocate memory for storing BC values of FW.\n");
    exit(0);
  }
  for(int i=0; i<graph->num_vertices; i++){
    bc_fw[i] = graph->bc[i];
  }

  // Normalize both BC values if toNormalize is set
  if(toNormalize){
    printf("\nNormalizing the BC values.\n");
    if(!normalizeBCvals(bc_bfs, num_vertices)){
      printf("\nError in main(): Unable to normalize BC values.\n");
      exit(0);
    }
    if(!normalizeBCvals(bc_fw, num_vertices)){
      printf("\nError in main(): Unable to normalize BC values.\n");
      exit(0);
    }
  }

  printf("\n\n\nOutput for the graph with (%d) vertices and in file (%s): \n\n", num_vertices, file);
  printf("\n-----------------------------------------------------------\n");
  printf("\n-----------------------------------------------------------\n");

  // Printing the BC values calculated using both the implementations
  printf("\nFollowing are the BC values for the graph using BFS:\n\n");
  for(int i=0; i<graph->num_vertices; i++){
    printf("vertex_id: %d \t\t BC_value: %f\n", i, bc_bfs[i]);
  }

  printf("\n-----------------------------------------------------------\n");

  printf("\n\nFollowing are the BC values for the graph using FW:\n\n");
  for(int i=0; i<graph->num_vertices; i++){
    printf("vertex_id: %d \t\t BC_value: %f\n", i, bc_fw[i]);
  }

  printf("\n-----------------------------------------------------------\n");

  /*

  //code for checking mismatch in BC values
  //commented out because almost all will mismatch by minute values for large graphs

  for(int i=0; i<graph->num_vertices; i++){
    if(bc_fw[i] != bc_bfs[i]){
      printf("\nMismatch in main(): BC value for BFS doesn't match with FW \t VertexId: %d, BC_BFS: %.30f, BC_FW: %.30f.\n", i, bc_bfs[i], bc_fw[i]);
    }
  }

  */

  // Printing the execution times for both the implementations
  printf("\n-----------------------------------------------------------\n");

  time_taken_bfs = ((double) (end_time_bfs-start_time_bfs)) / CLOCKS_PER_SEC;
  time_taken_fw = ((double) (end_time_fw-start_time_fw)) / CLOCKS_PER_SEC;

  printf("\n\nTime taken to calculate BC values using BFS: %lf seconds", time_taken_bfs);
  printf("\n\nTime taken to calculate BC values using FW: %lf seconds", time_taken_fw);

  printf("\n-----------------------------------------------------------\n");

  // fetching the total memory usage of the program
  struct rusage r_usage;
  getrusage(RUSAGE_SELF,&r_usage);

  // printing the memory usage in megabytes
  printf("\nMemory usage: %lf megabytes\n",(0.001)*r_usage.ru_maxrss);

  printf("\n-----------------------------------------------------------\n");

  //free the allocated memory
  if(!freeGraph(graph)){
    printf("\nError in main(): Unable to free the graph structure.\n");
    exit(0);
  }else{
    graph = NULL;
  }
  free(bc_bfs);
  bc_bfs = NULL;
  free(bc_fw);
  bc_fw = NULL;

  return 0;
}
