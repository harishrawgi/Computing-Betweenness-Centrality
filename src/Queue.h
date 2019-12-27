#ifndef QUEUE_H
#define QUEUE_H

//structure for queue
typedef struct queue{
  int *q;
  int front;
  int rear;
  int size;
} * Queue;

//function for creating an empty Queue with supplied size
//returns a pointer to the queue if succesfull, NULL otherwise
Queue createEmptyQueue(int size);

//function to check if queue is empty
//returns 1 if empty, 0 otherwise
int isEmptyQ(Queue Q);

//function to insert an element in queue with value as val
//returns 1 if succesfully inserted, 0 otherwise
int enqueue(Queue Q, int val);

//function to dequeue an element from the queue
//the dequeued element is stored in the parameter int* val
//returns 1 if succesfull, 0 otherwise
int dequeue(Queue Q, int* val);

//function to free the queue
int freeQueue(Queue Q);

#endif
