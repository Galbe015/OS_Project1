#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 100



int v1Index = 0;


typedef struct PCB_V1 PCB_V1;

typedef struct childPointer {
    struct childPointer* next;
    PCB_V1* child;
    int cID;
} childPointer;


typedef struct PCB_V1 {
    PCB_V1* parent;
    int index;
    struct childPointer* nextChild;
} PCB_V1;

PCB_V1* create(PCB_V1* p) {
    PCB_V1* pcb_q = (PCB_V1*)malloc(sizeof(PCB_V1));




    pcb_q->parent = p;
    pcb_q->index = ++v1Index;
    pcb_q->nextChild = NULL;

    if (p->nextChild == NULL) {
        childPointer* cp = (childPointer*)malloc(sizeof(childPointer));
        cp->child = pcb_q;
        cp->next = NULL;
        cp->cID =v1Index;
        p->nextChild = cp;
        printf("if\n");
    } else {
        printf("else\n");

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
    return pcb_q;

}


//working 2/1 1:00
void destroy(PCB_V1* p) {
    if (p == NULL) {
        printf("Attempted to destroy a null PCB.\n");
        return;
    }

    // Destroy all children of this process
    while (p->nextChild != NULL) {
        childPointer* cp = p->nextChild;
        p->nextChild = cp->next;
        destroy(cp->child); // Recursively destroy the child
        free(cp); // Free the childPointer structure
    }

    // Remove this PCB from its parent's child list
    if (p->parent != NULL) {
        childPointer* ParentCP = p->parent->nextChild;
        childPointer* Previous = NULL;
        while (ParentCP != NULL && ParentCP->child != p) {
            Previous = ParentCP;
            ParentCP = ParentCP->next;
        }
        if (ParentCP != NULL) {
            if (Previous == NULL) {
                // The child to remove is the first child
                p->parent->nextChild = ParentCP->next;
            } else {
                // The child to remove is not the first child
                Previous->next = ParentCP->next;
            }
            free(ParentCP);
        }
    }

    printf("Freeing PCB %d\n", p->index);
    free(p);
}




// Test program for Version 1
void testProgramV1() {
    printf("\nProcess ");
    PCB_V1* pcb0 = (PCB_V1*)malloc(sizeof(PCB_V1));
    pcb0->index=v1Index;
    pcb0->parent=NULL;
    pcb0->nextChild=NULL;

    PCB_V1* pcb1 = create(pcb0);
    PCB_V1* pcb2 = create(pcb0);
    PCB_V1* pcb3 = create(pcb0);
    PCB_V1* pcb4 = create(pcb2);
    PCB_V1* pcb5 = create(pcb2);



    //printf("\npcb2 son: %d\n", pcb2->nextChild->next->child->index);
    destroy(pcb0);

    //printf("\npcb2 son: %d\n", pcb2->nextChild->next->child->index);

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
