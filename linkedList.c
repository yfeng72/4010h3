#ifndef __LinkedList_C
#define __LinkedList_C

#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

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
    LinkedList *LinkedList = (LinkedList*) malloc( sizeof( LinkedList ) );
    LinkedList->head = newNode( -1 );
    LinkedList->tail = newNode( -2 );
    LinkedList->head->next = LinkedList->tail;
    LinkedList->head->prev = NULL;
    LinkedList->tail->prev = LinkedList->head;
    LinkedList->tail->next = NULL;
    return LinkedList;
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
