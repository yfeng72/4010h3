#ifndef __LinkedList_C
#define __LinkedList_C

#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

Node *newNode( int value ) {
    Node *newNode = (Node *) malloc( sizeof( Node ) );
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void append( LinkedList *list, int value ) {
    //insert at the end
    Node *inNode = newNode( value );
    Node *last = list->tail->prev;
    last->next = inNode;
    inNode->prev = last;
    inNode->next = list->tail;
    list->tail->prev = inNode;
}

int removeFirst( LinkedList *list ) {
    Node *firstNode = list->head->next;
    if ( firstNode == list->tail ) return -1;
    firstNode->next->prev = list->head;
    list->head->next = firstNode->next;
    int value = firstNode->value;
    free( firstNode );
    return value;
}

LinkedList *newList() {
    LinkedList *linkedList = (LinkedList *) malloc( sizeof( LinkedList ) );
    linkedList->head = newNode( -1 );
    linkedList->tail = newNode( -2 );
    linkedList->head->next = linkedList->tail;
    linkedList->head->prev = NULL;
    linkedList->tail->prev = linkedList->head;
    linkedList->tail->next = NULL;
    return linkedList;
}

void delete( LinkedList *list ) {
    Node *thisNode = list->head;
    while ( thisNode != NULL ) {
        Node *tmp = thisNode->next;
        free( thisNode );
        thisNode = tmp;
    }
    free( list );
}

#endif
