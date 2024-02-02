#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *array;
    int capacity;
    int top;
} Stack;

// Function to initialize the stack
void initializeStack(Stack *stack, int capacity) {
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
}



// Function to add an item to stack
void push(Stack *stack, int item) {
    if (stack->top == stack->capacity - 1) {
        // Increase capacity and reallocate
        stack->capacity *= 2;
        stack->array = realloc(stack->array, stack->capacity * sizeof(int));
    }
    stack->array[++stack->top] = item;
}

// Function to remove an item from stack
int pop(Stack *stack) {
    if (stack->top == -1) {
        printf("Stack Underflow\n");
        return -1; // Indicate error
    }
    return stack->array[stack->top--];
}

// Function to get the top item of stack
int top(Stack *stack) {
    if (stack->top == -1) {
        printf("Stack is Empty\n");
        return -1; // Indicate error
    }
    return stack->array[stack->top];
}

// Function to check if the stack is empty
int isEmpty(Stack *stack) {
    return stack->top == -1;
}

// Function to free stack memory
void destroyStack(Stack *stack) {
    free(stack->array);
    stack->array = NULL;
    stack->capacity = 0;
    stack->top = -1;
}

void fill(Stack *stack, int limit){

    for (int i = limit; i >= 0; i--) {
        push(stack, i);
    }

}




//int main() {
//    Stack stack;
//    initializeStack(&stack, 5); // Initialize stack with capacity 5
//
//    push(&stack, 10);
//    push(&stack, 20);
//
//    printf("Top element is %d\n", top(&stack));
//
//    pop(&stack);
//
//    printf("Top element is %d\n", top(&stack));
//
//    destroyStack(&stack);
//    return 0;
//}
