#ifndef PREDECESSORLIST_H
#define PREDECESSORLIST_H

//structure for the predecessor nodes
typedef struct predecessorNode{
  int vertexId;
  struct predecessorNode* next;
} * PredecessorNode;

//structure for the predecessor lists
typedef struct predecessorList {
  PredecessorNode head;

  //stores the number of elements in the list, this is used in BC computation
  int num_predecessors;
} * PredecessorList;

//function to create a predecessor node with supplied value
//returns pointer to the node if succesfull, NULL otherwise
PredecessorNode createPredecessorNode(int vertexId);

//function to add a new predecessor node to the list
//returns 1 if succesfull, 0 otherwise
int appendPredecessorList(PredecessorList pred, int vertexId);

//function to free up the predecessor list
//takes the pointer to the head of the lsit as the input
//returns 1 if succesfull, 0 otherwise
int freePredecessorList(PredecessorNode head);

#endif
