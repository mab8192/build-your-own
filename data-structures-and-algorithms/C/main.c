#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
    int size;
} LinkedList;

LinkedList* newLinkedList() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->size = 0;
    list->head = NULL;

    return list;
}

Node* newNode(int value) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->next = NULL;
    node->value = value;
    return node;
}

LinkedList* append(LinkedList* list, const int value) {
    if (list->head == NULL) {
        list->head = newNode(value);
        list->size = 1;
        return list;
    }

    Node* curr = list->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = newNode(value);
    list->size++;

    return list;
}

/**
 * Deletes the first occurance of the the value toDelete in list
*/
LinkedList* delete(LinkedList* list, const int toDelete) {
    Node* prev = NULL;
    Node* curr = list->head;

    while (curr != NULL) {
        if (curr->value == toDelete) {
            if (prev == NULL) {
                // Deleting the head
                list->head = curr->next;
                free(curr);
            } else {
                // Deleting somewhere else
                prev->next = curr->next;
                free(curr);
            }

            break;
        }
        prev = curr;
        curr = curr->next;
    }

    return list;
}

void freeList(LinkedList* list) {
    Node* curr = list->head;
    Node* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(list);
}

void printList(LinkedList* list) {
    Node* curr = list->head;

    printf("[HEAD");
    while (curr != NULL) {
        printf(" -> %d", curr->value);
        curr = curr->next;
    }

    printf("]\n");
}

int main(int argc, char** argv) {
    LinkedList* list = newLinkedList();

    for (int i = 0; i < 10; i++) {
        append(list, i);
    }

    printList(list);
    delete(list, 4);
    delete(list, 0);
    append(list, 4);
    append(list, 10);
    append(list, -1);
    printList(list);
    delete(list, 10);
    printList(list);

    // free memory
    freeList(list);

    return 0;
}