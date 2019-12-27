#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

//function for creating an empty Queue with supplied size
//returns a pointer to the queue if succesfull, NULL otherwise
Queue createEmptyQueue(int size){

  //validating inputs
  if(size<=0){
    printf("Error in createEmptyQueue(): invalid size input.\n");
    return 0;
  }

  //allocate memory for the queue structure
  Queue Q = (Queue) malloc(sizeof(struct queue));
  if(!Q){
    printf("Error in createEmptyQueue(): Unable to allocate memory for queue.\n");
    return 0;
  }

  //allocate memory for the actual queue
  Q->q = (int*) malloc(size*sizeof(int));
  if(!Q->q){
    printf("Error in createEmptyQueue(): Unable to allocate memory for queue.\n");
    return 0;
  }

  //initialize the queue front and rear indices, and size
  Q->front = -1;
  Q->rear = -1;
  Q->size = size;

  //return the pointer to the created queue
  return Q;
}

//function to check if queue is empty
//returns 1 if empty, 0 otherwise
int isEmptyQ(Queue Q){

  //validating inputs
  if(!Q){
    printf("Error in isEmptyQ(): NULL passed as input parameter.\n");
    return 0;
  }

  //check if Q is empty
  if(Q->rear == -1)
    return 1;
  else
    return 0;
}

//function to insert an element in queue with value as val
//returns 1 if succesfully inserted, 0 otherwise
int enqueue(Queue Q, int val){

  //validating inputs
  if(!Q){
    printf("Error in enqueue(): NULL passed as input parameter.\n");
    return 0;
  }

  //can't enqueue if Q is full
  if(Q->rear == Q->size - 1){
    printf("Error in enqueue(): Queue is full.\n");
    return 0;
  }

  //if first element
  if(Q->front == -1){
    Q->front = 0;
  }
  //insert the element at the end of the queue
  Q->rear++;
  Q->q[Q->rear] = val;

  return 1;
}

//function to dequeue an element from the queue
//the dequeued element is stored in the parameter int* val
//returns 1 if succesfull, 0 otherwise
int dequeue(Queue Q, int* val){

  //validating inputs
  if(!Q || !val){
    printf("Error in dequeue(): NULL passed as input parameter.\n");
    return 0;
  }

  //can't dequeue if Q is empty
  if(isEmptyQ(Q)){
    printf("Error in dequeue(): Queue is empty.\n");
    return 0;
  }

  //dequeue the element
  *val = Q->q[Q->front];
  Q->front++;

  //reset the queue if needed
  if(Q->front > Q->rear){
    //printf("Log in dequeue(): Resetting Queue.\n");
    Q->front = Q->rear = -1;
  }

  return 1;
}

//function to free the queue
int freeQueue(Queue Q){

  //validating the inputs
  if(!Q){
    printf("Error in freeQueue(): NULL passed as input parameter.\n");
    return 0;
  }

  //free the actual queue
  if(Q->q){
    free(Q->q);
    Q->q = NULL;
  }
  else{
    return 0;
  }

  //free the pointer to the structure
  free(Q);
  Q = NULL;

  return 1;

}
