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

void destroy(PCB_V1* p) {

    while (p->nextChild != NULL) {
        PCB_V1* currentChild = p->nextChild;
        p->nextChild = currentChild->nextChild;
        printf("\nProcess %d Destroyed\n", currentChild->index);
        destroy(currentChild); // Recursively destroy the child

    }
    printf("\nProcess %d destroyed\n", p->index);
    free(p);


}


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

    printf("\npcb1 son: %d\n", pcb1.nextChild->child->index);

    //destroy(pcb0);
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
