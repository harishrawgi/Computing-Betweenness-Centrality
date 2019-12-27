#include <stdio.h>
#include <stdlib.h>
#include "AdjList.h"

//function to create a adjacency list node with id as vertexId
//returns pointer to the created node if succesfull, NULL otherwise
AdjListNode createAdjListNode(int vertexId){

  //validate inputs
  if(vertexId < 0){
    printf("\nError in function createAdjListNode(): vertex range is non-negative.\n");
    return NULL;
  }

  //allocate memory for a new node
  AdjListNode node = (AdjListNode) malloc(sizeof(struct adjListNode));
  if(!node){
    printf("\nError in function createAdjListNode(): unable to allocate memory for vertex node.\n");
    return NULL;
  }

  //initialize the node with values
  node->vertexId = vertexId;
  node->next = NULL;

  return node;
}

//function to free the Adjacency List
int freeAdjList(AdjList adjList){

  //validating inputs
  if(!adjList){
    printf("\nError in freeAdjList(): NULL passed in as input parameter.");
    return 0;
  }

  //store the pointers for freeing up the nodes
  AdjListNode curr = adjList->head;
  AdjListNode temp = curr;

  while(curr){
    //store the pointer going to the next,
    //free up this stored pointer
    temp = curr;
    curr = curr->next;
    free(temp);
  }

  free(adjList);
  adjList = NULL;

  return 1;

}
