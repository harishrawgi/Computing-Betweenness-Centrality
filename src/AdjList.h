#ifndef ADJLIST_H
#define ADJLIST_H

//structure for a node of the adjacency list
typedef struct adjListNode {
  int vertexId;
  struct adjListNode* next;
} * AdjListNode;

//structure for the adjacency list
typedef struct adjList {
  AdjListNode head;
} * AdjList;

//function to create a adjacency list node with id as vertexId
//returns pointer to the created node if succesfull, NULL otherwise
AdjListNode createAdjListNode(int vertexId);

//function to free the Adjacency List
int freeAdjList(AdjList adjList);

#endif
