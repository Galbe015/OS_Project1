#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#define MAX_PROCESSES 100


typedef struct PCB_V1 PCB_V1;
typedef struct PCB_V2 PCB_V2;
const int maxChildren = 20;

int v1PID = 0;

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


typedef struct PCB_V2 {
    int parent;
    int first_child;
    int younger_sibling;
    int older_sibling;
    int index;
} PCB_V2;


PCB_V2 pcbArrayV2[MAX_PROCESSES];
Stack freeIndexStackV2;


void createV2(PCB_V2* p) {

    int newIndex = top(&freeIndexStackV2);

    pop(&freeIndexStackV2);

    //pcbArrayV2[newIndex] = *(PCB_V2*)malloc(sizeof(PCB_V2));
    pcbArrayV2[newIndex].first_child=-1;
    pcbArrayV2[newIndex].younger_sibling=-1;
    pcbArrayV2[newIndex].index=newIndex;

    pcbArrayV2[newIndex].parent=p->index;

    if(p->first_child==-1){
        pcbArrayV2[newIndex].older_sibling=-1;
        p->first_child= pcbArrayV2[newIndex].index;

    }else{

        int youngestChild = p->first_child;


        while(pcbArrayV2[youngestChild].younger_sibling != -1){
            youngestChild = pcbArrayV2[youngestChild].younger_sibling;

        }
        pcbArrayV2[newIndex].older_sibling=youngestChild;
        pcbArrayV2[youngestChild].younger_sibling=newIndex;

    }

}


void destroyV2(PCB_V2* p) {

    Stack children;

    initializeStack(&children,MAX_PROCESSES);

    int nextChild = p->first_child;
    while(nextChild != -1){

        push(&children,nextChild);
        nextChild = pcbArrayV2[nextChild].younger_sibling;
    }

    while(!isEmpty(&children)){
        destroyV2(&pcbArrayV2[pop(&children)]);
    }

    if(p->older_sibling!=-1){//destroy connection between  siblings
        if(p->younger_sibling!=-1){
            pcbArrayV2[p->older_sibling].younger_sibling=p->younger_sibling;
            pcbArrayV2[p->younger_sibling].older_sibling=p->older_sibling;


        }else{
            pcbArrayV2[p->older_sibling].younger_sibling=-1;
        }
    }else{//if he does not have an older sibling but has a younger sibling
        if(p->younger_sibling!=-1){
            pcbArrayV2[p->younger_sibling].older_sibling=-1;
            pcbArrayV2[p->parent].first_child=p->younger_sibling;

        }else{
            pcbArrayV2[p->parent].first_child=-1;

        }
    }

    pcbArrayV2[p->index].parent = -1;
    pcbArrayV2[p->index].first_child = -1;
    pcbArrayV2[p->index].younger_sibling = -1;
    pcbArrayV2[p->index].older_sibling = -1;

    destroyStack(&children);
    push(&freeIndexStackV2,p->index);

    pcbArrayV2[p->index].index = -1;


}


PCB_V1* create(PCB_V1* p) {
    PCB_V1* pcb_q = (PCB_V1*)malloc(sizeof(PCB_V1));

    pcb_q->parent = p;
    pcb_q->index = ++v1PID;
    pcb_q->nextChild = NULL;

    if (p->nextChild == NULL) {
        childPointer* cp = (childPointer*)malloc(sizeof(childPointer));
        cp->child = pcb_q;
        cp->next = NULL;
        cp->cID =v1PID;
        p->nextChild = cp;
    } else {

        childPointer* current = p->nextChild;
        while (current->next != NULL) {
            current = current->next;
        }

        childPointer* newChildPointer = (childPointer*)malloc(sizeof(childPointer));
        newChildPointer->child = pcb_q;
        newChildPointer->next = NULL;
        current->next = newChildPointer;

    }

    return pcb_q;

}


void destroyV1(PCB_V1* p) {
    if (p == NULL) {
        return;
    }

    // Destroy all children of this process
    while (p->nextChild != NULL) {
        childPointer* cp = p->nextChild;
        p->nextChild = cp->next;
        destroyV1(cp->child); // Recursively destroy the child
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

    free(p);
}


void testProgramV1() {
    srand((unsigned)time(NULL)); // Seed the random number generator

    // Define a maximum number of children a process can have


    // Repeat the test program in a loop
    for (int i = 0; i < 20; i++) {
        // Create root process
        PCB_V1* root = (PCB_V1*)malloc(sizeof(PCB_V1));
        root->index = v1PID;
        root->parent = NULL;
        root->nextChild = NULL;

        // Randomly decide how many children the root will have
        int numChildren = rand() % (maxChildren + 1); // Generates a number between 0 and maxChildren

        // Create children
        for (int j = 0; j < numChildren; j++) {
            create(root);
        }

        // Randomly destroy processes
        while (root->nextChild != NULL) {
            int destroyIndex = rand() % numChildren;
            childPointer* cp = root->nextChild;
            for (int k = 0; k < destroyIndex; k++) {
                if (cp->next == NULL) {
                    break;
                }
                cp = cp->next;
            }
            if (cp->child != NULL) {
                destroyV1(cp->child);
            }
        }

        // Destroy the root process last
        destroyV1(root);
    }
}


void resetPCBArrayV2() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcbArrayV2[i].parent = -1;
        pcbArrayV2[i].first_child = -1;
        pcbArrayV2[i].younger_sibling = -1;
        pcbArrayV2[i].older_sibling = -1;
        pcbArrayV2[i].index = -1;
    }
}


void testProgramV2() {
    srand((unsigned)time(NULL)); // Seed the random number generator

    // Repeat the test program in a loop
    for (int i = 0; i < 20; i++) {
        // Reset PCB Array and Stack for each iteration
        resetPCBArrayV2();
        fill(&freeIndexStackV2, MAX_PROCESSES);

        // Create root process
        pcbArrayV2[top(&freeIndexStackV2)].first_child = -1;
        pcbArrayV2[top(&freeIndexStackV2)].parent = -1;
        pcbArrayV2[top(&freeIndexStackV2)].younger_sibling = -1;
        pcbArrayV2[top(&freeIndexStackV2)].older_sibling = -1;
        pcbArrayV2[top(&freeIndexStackV2)].index = top(&freeIndexStackV2);
        pop(&freeIndexStackV2);

        // Randomly decide how many children the root will have and create them
        int numChildren = rand() % (maxChildren + 1);
        for (int j = 0; j < numChildren; j++) {
            createV2(&pcbArrayV2[0]);
        }

        // Randomly destroy some processes
        for (int j = 0; j < numChildren; j++) {
            int childIndex = rand() % MAX_PROCESSES;
            if (pcbArrayV2[childIndex].index != -1) { // Check if the process exists
                destroyV2(&pcbArrayV2[childIndex]);
            }
        }

        // Destroy the root process last
        destroyV2(&pcbArrayV2[0]);

        // Reset the PCB Array for the next iteration

    }
}


// Function to measure the execution time of a test program
double measureExecutionTime(void (*testProgram)()) {
    clock_t start_time = clock();
    testProgram();
    clock_t end_time = clock();
    return ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
}

int main() {
    //testProgramV1();

    // Initialize resources for Version 2
    initializeStack(&freeIndexStackV2, MAX_PROCESSES);
    fill(&freeIndexStackV2, MAX_PROCESSES);

    // Define the number of iterations for the test
    int iterations = 100000; // Adjust this number as needed
    double totalTimeV1 = 0.0, totalTimeV2 = 0.0;

    // Loop to run tests and measure time for both versions
    for (int i = 0; i < iterations; i++) {
        totalTimeV1 += measureExecutionTime(testProgramV1);
        totalTimeV2 += measureExecutionTime(testProgramV2);
    }


    // Calculate average time for each version
    double avgTimeV1 = totalTimeV1 / iterations;
    double avgTimeV2 = totalTimeV2 / iterations;

    // Output the results
    printf("Average time for Version 1: %f seconds\n", avgTimeV1);
    printf("Average time for Version 2: %f seconds\n", avgTimeV2);



    // Determine which version is faster and by what percentage
    double percentFaster;
    if (avgTimeV1 < avgTimeV2) {
        percentFaster = (avgTimeV2 - avgTimeV1) / avgTimeV2 * 100.0;
        printf("Version 1 is %.2f%% faster on average compared to Version 2.\n", percentFaster);
    } else if (avgTimeV2 < avgTimeV1) {
        percentFaster = (avgTimeV1 - avgTimeV2) / avgTimeV1 * 100.0;
        printf("Version 2 is %.2f%% faster on average compared to Version 1.\n", percentFaster);
    } else {
        printf("Both versions have the same average execution time.\n");
    }


    // Clean up resources
    destroyStack(&freeIndexStackV2);

    return 0;
}
