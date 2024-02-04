#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#define MAX_PROCESSES 100






typedef struct PCB_V1 PCB_V1;
typedef struct PCB_V2 PCB_V2;


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
        printf("\nCreating p %d he has no siblings\n",newIndex);
    }else{

        int youngestChild = p->first_child;

        printf("\nCreating p %d he siblings including: %d,",newIndex,youngestChild);

        while(pcbArrayV2[youngestChild].younger_sibling != -1){
            youngestChild = pcbArrayV2[youngestChild].younger_sibling;
            printf("p %d,",youngestChild);

        }
        pcbArrayV2[newIndex].older_sibling=youngestChild;
        pcbArrayV2[youngestChild].younger_sibling=newIndex;

    }

}


void destroyV2(PCB_V2* p) {



//    int nextChild = p->first_child;
//    while(nextChild != -1){
//        printf("\nhe had child %d destroying him how\n",nextChild);
//        destroyV2(&pcbArrayV2[nextChild]);
//        nextChild = pcbArrayV2[nextChild].younger_sibling;
//    }

/*
 * working but ineficent
    int childIndices[MAX_PROCESSES];
    int childCount = 0;
    int childIndex = p->first_child;
    while (childIndex != -1 && childCount < MAX_PROCESSES) {
        childIndices[childCount++] = childIndex;
        childIndex = pcbArrayV2[childIndex].younger_sibling;
    }

    // Destroy child PCBs using the collected indices
    for (int i = 0; i < childCount; i++) {
        destroyV2(&pcbArrayV2[childIndices[i]]);
    }
*/

    printf("\ndestroying p %d \n",p->index);

Stack children;

    initializeStack(&children,MAX_PROCESSES);


    int nextChild = p->first_child;
    while(nextChild != -1){
        printf("\nhe had child %d destroying him how\n",nextChild);
        //destroyV2(&pcbArrayV2[nextChild]);
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
   // free(p);

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


//working 2/1 1:00 (no array)
void destroyV1(PCB_V1* p) {
    if (p == NULL) {
        printf("Attempted to destroy a null PCB.\n");
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

    printf("Freeing PCB %d\n", p->index);
    free(p);
}




// Test program for Version 1
void testProgramV1() {
    printf("\nProcess ");
    PCB_V1* pcb0 = (PCB_V1*)malloc(sizeof(PCB_V1));
    pcb0->index=v1PID;
    pcb0->parent=NULL;
    pcb0->nextChild=NULL;

    PCB_V1* pcb1 = create(pcb0);
    PCB_V1* pcb2 = create(pcb0);
    PCB_V1* pcb3 = create(pcb0);
    PCB_V1* pcb4 = create(pcb2);
    PCB_V1* pcb5 = create(pcb2);



    //printf("\npcb2 son: %d\n", pcb2->nextChild->next->child->index);
    destroyV1(pcb0);

    //printf("\npcb2 son: %d\n", pcb2->nextChild->next->child->index);

}





// Test program for Version 2
void testProgramV2() {
    // Implement test program for Version 2

   // pcbArrayV2[top(&freeIndexStackV2)] = *(PCB_V2*)malloc(sizeof(PCB_V2));
    pcbArrayV2[top(&freeIndexStackV2)].first_child=-1;
    pcbArrayV2[top(&freeIndexStackV2)].parent=-1;
    pcbArrayV2[top(&freeIndexStackV2)].younger_sibling=-1;
    pcbArrayV2[top(&freeIndexStackV2)].older_sibling=-1;
    pop(&freeIndexStackV2);


    createV2(&pcbArrayV2[0]);
    createV2(&pcbArrayV2[0]);
    createV2(&pcbArrayV2[0]);



    destroyV2(&pcbArrayV2[0]);



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
    initializeStack(&freeIndexStackV2, MAX_PROCESSES);
    fill(&freeIndexStackV2,MAX_PROCESSES);


    testProgramV2();



    destroyStack(&freeIndexStackV2);

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
