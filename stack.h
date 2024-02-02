//
// Created by Gabri on 2/2/2024.
//

#ifndef PROGRAMS_STACK_H
#define PROGRAMS_STACK_H

#endif //PROGRAMS_STACK_H


// stack.h
#ifndef STACK_H
#define STACK_H

typedef struct {
    int *array;
    int capacity;
    int top;
} Stack;

void initializeStack(Stack *stack, int capacity);
void push(Stack *stack, int item);
int pop(Stack *stack);
int top(Stack *stack);
int isEmpty(Stack *stack);
void destroyStack(Stack *stack);
void fill(Stack *stack, int limit);

#endif // STACK_H
