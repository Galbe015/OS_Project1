#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 100

//typedef struct PCBNode {
//    struct PCBNode* next; // Pointer to the next child node
//} PCBNode;

//typedef struct PCB_V1 {
//    int parent;
//    int index;
//    struct PCV_V1* nextChild;
//
//} PCB_V1;
//
//void create(PCB_V1* p){
//    PCB_V1* pcb_q = (PCB_V1*) malloc(sizeof (PCB_V1));
//    pcb_q->parent = p->index;
//    pcb_q->nextChild = NULL;
//
//    if(p->nextChild==NULL){
//        p->nextChild = pcb_q;
//    }else{
//        while(p.)
//    }
//
//
//
//}

int v1Index = 0;


typedef struct PCB_V1 PCB_V1;

typedef struct childPointer {
    struct childPointer* next;
    PCB_V1* child;
} childPointer;


typedef struct PCB_V1 {
    int parent;
    int index;
    struct childPointer* nextChild;
} PCB_V1;

PCB_V1 create(PCB_V1* p) {
    PCB_V1* pcb_q = (PCB_V1*)malloc(sizeof(PCB_V1));




    pcb_q->parent = p->index;
    pcb_q->index = ++v1Index;
    pcb_q->nextChild = NULL;

    if (p->nextChild == NULL) {
        childPointer* cp = (childPointer*)malloc(sizeof(childPointer));
        cp->child = pcb_q;
        cp->next = NULL;
        p->nextChild = cp;
        printf("if\n");
    } else {
        printf("else\n");
//        PCB_V1* current = p->nextChild->child;
//
//        while (current->nextChild->child != NULL) {
//            printf("%d is not null",current->nextChild->child->index);
//            current = current->nextChild->child;
//        }
//
//        current->nextChild->child = pcb_q;
//
        childPointer* current = p->nextChild;
        while (current->next != NULL) {
            current = current->next;
        }

        childPointer* newChildPointer = (childPointer*)malloc(sizeof(childPointer));
        newChildPointer->child = pcb_q;
        newChildPointer->next = NULL;
        current->next = newChildPointer;

    }

    printf("\nProcess %d Created\n", pcb_q->index);
    return *pcb_q;

}
//
//void destroy(PCB_V1 * p) {
//
//    if(p->nextChild==NULL){
//        free(p);
//    }else {
//        childPointer* next = p->nextChild;
//        while (next != NULL) {
//
//            free(next->child);
//
//            if (next->next!=NULL){
//                next= next->next;
//            }
//
//        }
//
//    }
//void destroy(PCB_V1* p) {
//    printf("Destroying process %d...\n", p->index); // Debug message for the current process
//
//    childPointer* current = p->nextChild;
//    while (current != NULL) {
//        childPointer* next = current->next;
//        printf("Destroying child process %d of parent %d...\n", current->child->index, p->index); // Debug message for each child
//        destroy(current->child);  // Recursively destroy the child PCB
//        free(current);            // Free the childPointer node
//        current = next;           // Move to the next childPointer
//    }
//
//    free(p);  // Finally, free the PCB itself
//    printf("Process %d destroyed.\n", p->index); // Confirmation message
//}

void destroy(PCB_V1* p) {
    // First, destroy all children of this process
    childPointer* current = p->nextChild;
    while (current != NULL) {
        childPointer* temp = current->next; // Save the next pointer
        destroy(current->child); // Recursively destroy the child
        free(current); // Free the childPointer structure
        current = temp; // Move to the next child
    }

    // After all children are destroyed, destroy this process
    printf("\nProcess %d destroyed\n", p->index);
    free(p);
}


//    while (p->nextChild != NULL) {
//        childPointer* currentChild = p->nextChild;
//        childPointer* next;
//        while(currentChild != NULL){
//            next=currentChild->next;
//            destroy(next)
//        }
//
//        p->nextChild = currentChild->nextChild;
//        printf("\nProcess %d Destroyed\n", currentChild->index);
//        destroy(currentChild); // Recursively destroy the child
//
//    }
//    printf("\nProcess %d destroyed\n", p->index);
//    free(p);


//}


// Test program for Version 1
void testProgramV1() {
    printf("\nProcess ");
    PCB_V1* pcb0 = (PCB_V1*)malloc(sizeof(PCB_V1));
    pcb0->index=v1Index;
    pcb0->parent=-1;
    pcb0->nextChild=NULL;

    PCB_V1 pcb1 = create(pcb0);
    PCB_V1 pcb2 = create(&pcb1);
    PCB_V1 pcb3 = create(pcb0);
    PCB_V1 pcb4 = create(&pcb2);

    printf("\npcb1 son: %d\n", pcb1.nextChild->child->index);

    destroy(pcb0);
    //printf("\npcb2 son: %d\n", pcb0->nextChild->next->child->index);

}



// Test program for Version 2
void testProgramV2() {
    // Implement test program for Version 2
}

// Function to measure the execution time of a test program
double measureExecutionTime(void (*testProgram)()) {
    clock_t start_time = clock();
    testProgram();
    clock_t end_time = clock();
    return ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
}

int main() {
    // Initialize PCB arrays for both versions
    // ...

testProgramV1();


    return 0;
}
//
//int main() {
//    // Initialize PCB arrays for both versions
//    // ...
//
//    // Repeat the test program in a long loop and measure the execution time
//    int numIterations = 100000; // Adjust the number of iterations as needed
//    double timeVersion1 = 0.0;
//    double timeVersion2 = 0.0;
//
//    for (int i = 0; i < numIterations; ++i) {
//        // Version 1
//        timeVersion1 += measureExecutionTime(testProgramV1);
//
//        // Version 2
//        timeVersion2 += measureExecutionTime(testProgramV2);
//    }
//
//    // Display the average execution time for each version
//    printf("Average execution time for Version 1: %.6f seconds\n", timeVersion1 / numIterations);
//    printf("Average execution time for Version 2: %.6f seconds\n", timeVersion2 / numIterations);
//
//    return 0;
//}
