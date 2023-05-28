#ifndef __STD_STRUCT_H__
#define __STD_STRUCT_H__

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

// Caller is responsible for passing valid pointers to functions

typedef struct DL_Ptr {
    struct DL_Ptr *prev;
    struct DL_Ptr *next;
} DL_Node;

typedef struct SL_Ptr {
    struct SL_Ptr *next;
} SL_Node;

void DeleteNextDLNode(DL_Node *Node) {
    if (Node->next) {
        DL_Node *nextNode = (DL_Node *)(Node->next);
        Node->next = nextNode->next;
        if (nextNode->next) {
            ((DL_Node *)(nextNode->next))->prev = Node;
        }
    }
    return;
}

void InsertDLNode(DL_Node *Node, DL_Node *TargetNode) {
    if (Node->next) {
        DL_Node *nextNode = (DL_Node *)(Node->next);
        nextNode->prev = TargetNode;
        TargetNode->next = Node->next;
    }
    Node->next = TargetNode;
    TargetNode->prev = Node;
    return;
}

void DeleteNextSLNode(SL_Node *Node) {
    if (Node->next) {
        Node->next = ((SL_Node *)(Node->next))->next;
    }
    return;
}

void InsertSLNode(SL_Node *Node, SL_Node *TargetNode) {
    TargetNode->next=Node->next;
    Node->next=TargetNode;
    return;
}

/*
something for testing:

void PrintElement(DL_Node *head) {
    if (head == NULL)
        return;

    DL_Node *current=head;
    int count=0;
    while (current) {
        printf("%p\n", current);
        count++;
        current=current->next;
    }
    printf("Total nodes printed: %d\n", count);
}
*/


/*
test:

int main() {
    DL_Node n1, n2, n3, n4;
    printf("n1, n2, n3, n4: %p, %p, %p, %p\n\n", &n1, &n2, &n3, &n4);

    n2.prev=&n1;
    n2.next=NULL;
    n1.next=&n2;

    PrintElement(&n1);

    return 0;
}
*/

#endif