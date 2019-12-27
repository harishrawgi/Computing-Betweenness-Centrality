#ifndef STACK_H
#define STACK_H

//structure for queue
typedef struct stack{
  int *stack;
  int top;
  int size;
} * Stack;

//function for creating an empty Stack with supplied size
//returns a pointer to the stack if succesfull, NULL otherwise
Stack createEmptyStack(int size);

//function to check if stack is empty
//returns 1 if empty, 0 otherwise
int isEmptyStack(Stack S);

//function to check if stack is full
//returns 1 if full, 0 otherwise
int isFullStack(Stack S);

//function to push an element with value as val onto the top of stack
//returns 1 if succesfull, 0 otherwise
int push(Stack S, int val);

//function to pop an element from the top of stack
//popped element is stored in the parameter int* val
//returns 1 if succesfull, 0 otherwise
int pop(Stack S, int* val);

//function to free the stack
int freeStack(Stack S);

#endif
