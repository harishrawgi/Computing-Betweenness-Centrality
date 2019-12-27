#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

//function for creating an empty Stack with supplied size
//returns a pointer to the stack if succesfull, NULL otherwise
Stack createEmptyStack(int size){

  //validating inputs
  if(size<=0){
    printf("Error in createEmptyStack(): invalid size input.\n");
    return 0;
  }

  //allocate memory for the stack structure
  Stack S = (Stack) malloc(sizeof(struct stack));
  if(!S){
    printf("Error in createEmptyStack(): Unable to allocate memory for stack.\n");
    return 0;
  }

  //allocate memory for the actual stack
  S->stack = (int*) malloc(size*sizeof(int));
  if(!S->stack){
    printf("Error in createEmptyStack(): Unable to allocate memory for stack.\n");
    return 0;
  }

  //initialize the top index and size
  S->top = -1;
  S->size = size;

  return S;
}

//function to check if stack is empty
//returns 1 if empty, 0 otherwise
int isEmptyStack(Stack S){

  //validating inputs
  if(!S){
    printf("Error in isEmptyStack(): NULL passed as input parameter.\n");
    return 0;
  }

  //check if the stack is empty and return
  return S->top == -1;
}

//function to check if stack is full
//returns 1 if full, 0 otherwise
int isFullStack(Stack S){

  //validating inputs
  if(!S){
    printf("Error in isFullStack(): NULL passed as input parameter.\n");
    return 0;
  }

  //check if Stack is full and return
  return S->top == S->size - 1;
}

//function to push an element with value as val onto the top of stack
//returns 1 if succesfull, 0 otherwise
int push(Stack S, int val){

  //validating inputs
  if(!S){
    printf("Error in push(): NULL passed as input parameter.\n");
    return 0;
  }

  //can't push if stack is full
  if(isFullStack(S)){
    printf("Error in push(): Stack is full.\n");
    return 0;
  }

  //push the element on the top of stack
  S->stack[++(S->top)] = val;
  return 1;
}

//function to pop an element from the top of stack
//popped element is stored in the parameter int* val
//returns 1 if succesfull, 0 otherwise
int pop(Stack S, int* val){

  //validating inputs
  if(!S || !val){
    printf("Error in pop(): NULL passed as input parameter.\n");
    return 0;
  }

  //can't pop if stack is empty
  if(isEmptyStack(S)){
    printf("Error in push(): Stack is empty.\n");
    return 0;
  }

  //pop the top element from the stack and update top index
  *val = S->stack[S->top--];
  return 1;
}

//function to free the stack
int freeStack(Stack S){

  //validating inputs
  if(!S){
    printf("Error in freeStack(): NULL passed as input parameter.\n");
    return 0;
  }

  //free the actual stack
  if(S->stack){
    free(S->stack);
    S->stack = NULL;
  }
  else{
    return 0;
  }

  //free the pointer to the stack structure
  free(S);
  S = NULL;

  return 1;
}
