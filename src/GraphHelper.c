#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GraphHelper.h"
#include "Graph.h"
#include "Stack.h"
#include "Queue.h"
#include "PredecessorList.h"

//function to create an empty graph (no edges) with supplied number of vertices
//returns pointer to the graph if succesfull, NULL otherwise
Graph createEmptyGraph(int num_vertices){

  //validating inputs
  if(num_vertices <= 0){
    printf("\nError in function createGraph(): number of vertices passed as non-positive.\n");
    return NULL;
  }

  //allocate memory for the graph structure
  Graph graph = (Graph) malloc(sizeof(struct graph));
  if(!graph){
    printf("\nError in function createGraph(): failed to allocate memory for the graph.\n");
    return NULL;
  }

  //set the number of vertices and edges
  graph->num_vertices = num_vertices;
  graph->num_edges = 0;

  //allocate the memory for the BC values and initialize them to 0
  graph->bc = (float*) malloc(num_vertices * sizeof(float));
  if(!graph->bc){
    printf("\nError in function createGraph(): failed to allocate memory for the num_shortest_paths array.\n");
    return NULL;
  }
  for(int i=0; i<num_vertices; i++){
    graph->bc[i] = 0;
  }

  //allocate memory for the array of adjacency lists
  graph->vertexArray = (AdjList) malloc(num_vertices * sizeof(struct adjList));
  if(!graph->vertexArray){
    printf("\nError in function createGraph(): failed to allocate memory for the vertex array.\n");
    return NULL;
  }
  for(int i=0; i<num_vertices; i++){
    (graph->vertexArray[i]).head = NULL;
  }

  //return the created empty graph
  return graph;
}

//function to add a new edge to the graph with source as src and destination as dest
//adds two edges as the graph is undirected
int addEdgeToGraph(Graph graph, int src, int dest){

  //validating inputs
  if(!graph){
    printf("\nError in function addEdgeToGraph(): NULL passed as an input parameter.\n");
    return 0;
  }
  if(src < 0 || dest < 0){
    printf("\nError in function addEdgeToGraph(): range of vertices is non-negative.\n");
    return 0;
  }

  //creating a new node to be added into the adjacency list of src vertex
  AdjListNode node = createAdjListNode(dest);
  if(!node){
    printf("\nError in function addEdgeToGraph(): unable to allocate memory for vertex node.\n");
    return 0;
  }
  //add the new node to the list
  node->next = graph->vertexArray[src].head;
  graph->vertexArray[src].head = node;

  //since graph is undirected, we also add the edge with reverse src and dest
  AdjListNode revNode = createAdjListNode(src);
  if(!revNode){
    printf("\nError in function addEdgeToGraph(): unable to allocate memory for vertex node.\n");
    return 0;
  }
  revNode->next = graph->vertexArray[dest].head;
  graph->vertexArray[dest].head = revNode;

  //increment the number of edges in the graph by 2
  graph->num_edges += 2;

  return 1;
}

//helper function which performs BC computation for all vertices corresponding to startVertexId as the source
//BC values which are computed are added to the graph which is taken as input
//returns 1 if succesfull, 0 otherwise
int bfsBC(Graph graph, int startVertexId){

  //validating inputs
  if(!graph){
    printf("\nError in bfsBC(): NULL passed as input parameter.\n");
    return 0;
  }
  if(startVertexId < 0){
    printf("\nError in bfsBC(): vertices range is non-negative.\n");
    return 0;
  }

  //nosp: Number of Shortest Paths
  //nosp is a real valued array for storing the number of shortest paths through a node
  //these values are all corresponding to the paths where startVertexId is the src node
  float* nosp = (float*) malloc(graph->num_vertices * sizeof(float));
  if(!nosp){
    printf("\nError in bfsBC(): unable to allocate memory for nosp array.\n");
    return 0;
  }
  //initialize the values to 0
  for(int i=0; i<graph->num_vertices; i++){
    nosp[i] = 0;
  }

  //n_pred is used to keep track of number of predecessors each node has
  //this would be useful in dividing up the BC values, which means that
  //it would be used to count the contributions of the number of shortest paths
  //going through a vertex towards it's predecessors
  float* n_pred = (float*) malloc(graph->num_vertices * sizeof(float));
  if(!n_pred){
    printf("\nError in bfsBC(): unable to allocate memory for n_pred array.\n");
    return 0;
  }
  //initialize the values to 0
  for(int i=0; i<graph->num_vertices; i++){
    n_pred[i] = 0;
  }

  //the src has n_pred value of 1
  n_pred[startVertexId] = 1;

  //creating a dist array to keep track of distances from the start vertex
  int* dist = (int*) malloc(graph->num_vertices * sizeof(int));
  if(!dist){
    printf("\nError in bfsBC(): unable to allocate memory for dist array.\n");
    return 0;
  }
  //initialize all of them to INFINITY
  for(int i=0; i<graph->num_vertices; i++){
    dist[i] = INFINITY;
  }

  //create an array of predecessor lists, one list for each vertex
  PredecessorList pred = (PredecessorList) malloc(graph->num_vertices * sizeof(struct predecessorList));
  if(!pred){
    printf("\nError in bfsBC(): unable to allocate memory for predecessorList.\n");
    return 0;
  }
  //initialize the lists to be empty and have 0 predecessors
  for(int i=0; i<graph->num_vertices; i++){
    pred[i].head = NULL;
    pred[i].num_predecessors = 0;
  }


  //visited[startVertexId] = 1;

  //distance of the start vertex to itself is 0
  dist[startVertexId] = 0;

  //create an empty Queue for BFS
  Queue Q = createEmptyQueue(graph->num_vertices);
  if(!Q){
    printf("\nError in bfsBC(): unable to create empty queue.\n");
    return 0;
  }

  //enqueue the start vertex to begin BFS
  if(!enqueue(Q, startVertexId)){
    printf("\nError in bfsBC(): unable to enque vertex (%d).\n", startVertexId);
    return 0;
  }

  //create an empty Stack; this will be used in BC computation
  Stack S = createEmptyStack(graph->num_vertices);
  if(!S){
    printf("\nError in bfsBC(): unable to create empty stack.\n");
    return 0;
  }

  //BFS segment
  //here we also populate the stack which is later used for the BC computation
  while(!isEmptyQ(Q)){

    //dequeue a vertex
    int v;
    if(!dequeue(Q, &v)){
      printf("\nError in bfsBC(): unable to deque.\n");
      return 0;
    }

    //push it onto the stack; to be used later
    push(S,v);

    //printf("\n-----------------Pushed vertex (%d) onto the stack-----------------\n", v);

    //iterate over all of v's neighbours

    struct adjList adj = graph->vertexArray[v];
    AdjListNode curr = adj.head;

    //AdjListNode curr = graph->vertexArray[v].head;
    while(curr){

      //printf("\nCurrently iterating over neighbour (%d) of vertex (%d)", curr->vertexId, v);
      //update the dist of the neighbour if possible
      if(dist[curr->vertexId] == INFINITY){
        dist[curr->vertexId] = dist[v] + 1;
        //printf("\nUpdated dist of (%d) vertex to (%d)", curr->vertexId, dist[curr->vertexId]);

        //enqueue the neighbour if it is being visited first time
        if(!enqueue(Q, curr->vertexId)){
          printf("\nError in bfsBC(): unable to enque vertex (%d).\n", startVertexId);
          return 0;
        }
      }

      //if v is the parent of the neighbour in the BFS tree
      if(dist[curr->vertexId] == dist[v] + 1){// && v!=startVertexId){

        n_pred[curr->vertexId] += n_pred[v];

        //append v to the predecessor list of the neighbour
        if(!appendPredecessorList(&pred[curr->vertexId], v)){
          printf("\nError in bfsBC(): unable to append predecessor.\n");
          return 0;
        }
      }
      curr = curr->next;
    }
  }
  //BFS completed, we have the BFS tree form stored in the stack

/*
  printf("\nSimga values are: ");
  for(int i=0; i<graph->num_vertices; i++){
    printf("\t %f", n_pred[i]);
  }
*/

  //BC computation segment
  //We use the earlier populated stack to compute BC values in a bottom-up manner
  //the code will start popping leaves and move up the tree, updating the BC values
  while(!isEmptyStack(S)){

    //pop the top element of the stack
    int v;
    if(!pop(S, &v)){
      printf("\nError in bfsBC(): unable to pop from stack.\n");
      return 0;
    }

    //printf("\nPopped vertex: %d\n", v);

    //if v is the start vertex, this means we have reached the root of the tree
    //BC value for this will be 0, so break the loop
    if(v==startVertexId) break;

    //iterate the predecessors of v to calculate their BC values using v
    PredecessorNode curr = pred[v].head;
    while(curr){

      if(curr->vertexId == startVertexId) { curr = curr->next; continue;}
      //v contributes nosp(number of shortest path through v) + 1 (shortest path to v from it's predecessor)
      //this has to be normalized by the number of predecessors v has
      float normalizedNosp = (nosp[v] + 1)*(n_pred[curr->vertexId]/n_pred[v]);
      //printf("\nThe normalizedNosp is: %f\n", normalizedNosp);
      nosp[curr->vertexId] += normalizedNosp;

      //printf("\nAdded nosp value (%f) to the vertex (%d)", normalizedNosp, curr->vertexId);

      curr = curr->next;
    }
  }
  //BC computation complete

/*
  printf("\n-------------------Final nosp values after this BFS-----------------------\n");
  for(int i=0; i<graph->num_vertices; i++){
    printf("\nnosp_val of (%d): %f", i, nosp[i]);
  }
*/

  //add the caluclated BC values corresponding to paths where startVertexId is the src to the graph
  for(int i=0; i<graph->num_vertices; i++){
    graph->bc[i] += nosp[i];
  }


  free(nosp);
  free(dist);
  free(n_pred);

  for(int i=0; i<graph->num_vertices; i++){
    if(pred[i].head){
      freePredecessorList(pred[i].head);
    }
  }
  free(pred);
  pred = NULL;
  if(!freeQueue(Q)){
    printf("\nError in bfsBC(): Failed to free the queue.\n");
    return 0;
  }
  if(!freeStack(S)){
    printf("\nError in bfsBC(): Failed to free the stack.\n");
    return 0;
  }

  return 1;
}

//function to create an adjacency matrix from the given undirected graph
AdjacencyMatrix createAdjMat(Graph graph){

  //validating inputs
  if(!graph){
    printf("\nError in function createAdjMat(): NULL passed as an input parameter.\n");
    return NULL;
  }

  //create an empty adjacency matrix
  AdjacencyMatrix adjMat = createEmptyAdjacencyMatrix(graph->num_vertices);
  if(!adjMat){
    printf("\nError in function createAdjMat(): Unable to create an empty adjacencyMatrix.\n");
    return NULL;
  }

  // set the initial values for the adjacency matrix
  // here we set infinity as teh initial value, these will become one in next segment if the vertices are adjacent
  // 0 is set for the diagonal values
  for(int i=0;i<graph->num_vertices;i++){
    for(int j=0;j<graph->num_vertices;j++){
      if(i!=j) adjMat->adj[i][j] = INFINITY;
      else adjMat->adj[i][j] = 0;
    }
  }

  //set the adjacency matrix entries to 1 if the vertices are adjacent in the graph
  for(int i=0; i<graph->num_vertices;i++){

    //fetch the adjacency list of the vertex
    struct adjList adj = graph->vertexArray[i];
    AdjListNode curr = adj.head;

    //iterate over the adjacency list of the vertex and set matrix entries as 1 for all vertices in the list
    while(curr){
      adjMat->adj[i][curr->vertexId] = 1;
      curr= curr->next;
    }
  }

  //return the created adjacency matrix
  return adjMat;

}

//helper function to calculate the BC values for the graph from the adjacency matrix using FW
int calculateBCFW_helper(AdjacencyMatrix dist, Graph graph){

  //validating inputs
  if(!graph || !dist){
    printf("\nError in function calculateBCFW_helper(): NULL passed as an input parameter.\n");
    return 0;
  }

  //store the number of vertices ina  variable for ease
  int v = graph->num_vertices;

  //count matrix for storing the number of shortest paths between the vertices
  float *count[v];
  for (int i=0; i<v; i++){
    count[i] = (float *)malloc(v * sizeof(float));
    if(!count[i]){
      printf("\nError in function calculateBCFW_helper(): Unable to allocate memory for count matrix.\n");
      return 0;
    }
  }

  //initializing the count matrix
  for(int i=0;i<v;i++){
    for(int j=0;j<v;j++){
      //if the vertices are adjacent, then the number of shortest path between them is 1
      if((dist->adj[i][j] == 1)){
        count[i][j] = 1;
      }
      //otherwise we intiialize the count to zero
      else{
        count[i][j] = 0;
      }
    }
  }


  //these variables are used to store intermediate results
  int dist_k_i, dist_k_j, dist_via_k;
  float count_k_i, count_k_j;

  //Floyd-Warshall segment, we also calculate number of shortest paths in the same segment
  //For optimum execution time, we use the symmetric property of the adj matrix
  for(int k=0; k<v; k++){
    for(int i=0; i<v; i++){

      //we are not using the lower triangular part of the matrix
      //so we need to take symmetric values from the upper traingular part only
      dist_k_i = (k<i)? dist->adj[k][i]:dist->adj[i][k];

      //skipping the third loop if the following is true
      if(k == i || dist_k_i == INFINITY){
        continue;
      }

      //thrid loop
      for(int j=i+1; j<v; j++){

        //again we only take values from the upper triangular part
        dist_k_j = (k<j)? dist->adj[k][j]:dist->adj[j][k];

        //skip if the following is true
        if(k==j || i==j || dist_k_j == INFINITY){
          continue;
        }

        dist_via_k = dist_k_i + dist_k_j;

        //if dist from i to j via k, is the same as the shortes path between them,
        //then we update the count of the number of shortest paths between i and j
        //this happens because we found new shortest paths between i and j through k
        if(dist->adj[i][j] == dist_k_i + dist_k_j){

          //only upper-triangular values
          count_k_j = (k<j)? count[k][j]:count[j][k];
          count_k_i = (k<i)? count[k][i]:count[i][k];

          //update the count
          count[i][j] += count_k_i*count_k_j;
        }
        //if we found a path which is shorter, we must update the dist value of i and j (Traditional FW)
        //but we also need to reset teh count value, as the previous count values don't hold anymore
        //since we found a new shorter route
        else if(dist->adj[i][j] > dist_via_k){

          //only upper-triangular values
          count_k_j = (k<j)? count[k][j]:count[j][k];
          count_k_i = (k<i)? count[k][i]:count[i][k];

          //update the dist and reset the count value
          dist->adj[i][j] = dist_via_k;
          count[i][j] = count_k_i*count_k_j;
        }
      }
    }
  }
  //end of FW segment

  //Start of BC value calculation segment
  for(int k=0; k<v; k++){
    for(int i=0; i<v; i++){

      //same as FW segment
      dist_k_i = (k<i)? dist->adj[k][i]:dist->adj[i][k];
      if(k == i || dist_k_i == INFINITY){
        continue;
      }

      for(int j=i+1; j<v; j++){

        //same as FW segement
        dist_k_j = (k<j)? dist->adj[k][j]:dist->adj[j][k];
        if(k==j || i==j || dist_k_j == INFINITY){
          continue;
        }

        dist_via_k = dist_k_i + dist_k_j;

        //if k lies on the shortest path from i to j
        //then, we update it's BC value
        if(dist->adj[i][j] == dist_via_k){

          //only upper-triangular values are used
          count_k_j = (k<j)? count[k][j]:count[j][k];
          count_k_i = (k<i)? count[k][i]:count[i][k];

          //normalize the count of shortest path with total number of shortest path
          float normalized_bcval = (count_k_i*count_k_j)/((float)count[i][j]);

          //add it to the overall BC value of k
          graph->bc[k] += normalized_bcval;
        }
      }
    }
  }
  //end of BC value calculation segment

  //free the count matrix
  if(count){
    for(int i=0; i<v; i++){
      if(count[i]){
        free(count[i]);
        count[i] = NULL;
      }
    }
  }

  //return 1 if succesfull
  return 1;
}
