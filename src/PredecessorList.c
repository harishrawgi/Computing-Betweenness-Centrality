#include <stdio.h>
#include <stdlib.h>
#include "GraphHelper.h"

//function to create a predecessor node with supplied value
//returns pointer to the node if succesfull, NULL otherwise
int appendPredecessorList(PredecessorList pred, int vertexId){

  //validating inputs
  if(!pred){
    printf("\nError in function appendPredecessorList(): NULL passed as input parameter.\n");
    return 0;
  }
  if(vertexId < 0){
    printf("\nError in function appendPredecessorList(): vertex range is non-negative.\n");
    return 0;
  }

  //create a new node to add to the predecessor list
  PredecessorNode node = createPredecessorNode(vertexId);
  if(!node){
    printf("\nError in function appendPredecessorList(): Unable to create predecessorNode.\n");
    return 0;
  }

  //add the node to the front of the list
  node->next = pred->head;
  pred->head = node;

  //increase the count of predecessors by 1
  pred->num_predecessors++;

  return 1;
}

//function to add a new predecessor node to the list
//returns 1 if succesfull, 0 otherwise
PredecessorNode createPredecessorNode(int vertexId){

  //validating input
  if(vertexId < 0){
    printf("\nError in function createPredecessorNode(): vertex range is non-negative.\n");
    return NULL;
  }

  //allocate memory for a new node
  PredecessorNode node = (PredecessorNode) malloc(sizeof(struct predecessorNode));
  if(!node){
    printf("\nError in function createPredecessorNode(): unable to allocate memory for vertex node.\n");
    return NULL;
  }
  //initialize the node with corresponding values
  node->vertexId = vertexId;
  node->next = NULL;

  return node;
}

//function to free up the predecessor list
//takes the pointer to the head of the lsit as the input
//returns 1 if succesfull, 0 otherwise
int freePredecessorList(PredecessorNode head){
  //validating inputs
  if(!head){
    printf("\nError in function freePredecessorList(): NULL passed as input parameter.\n");
    return 0;
  }

  //store the pointers for freeing up the nodes
  PredecessorNode curr = head;
  PredecessorNode temp = curr;

  while(curr){
    //store the pointer going to the next,
    //free up this stored pointer
    temp = curr;
    curr = curr->next;
    free(temp);
  }

  return 1;
}
