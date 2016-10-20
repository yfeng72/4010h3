#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

/**
 * Instantiates and builds the network
 */
LinkedList **createNetwork( int numNodes );

/**
 * Returns the number of connections a node has.
 */
int getDegree( LinkedList *connections );

/**
 * Save the network to a .csv file
 */
void storeNetwork( char filename[] );

/**
 * Produces a histogram based on network size formatted as:
 * "Node ID, connection1, connection2, ..."
 */
void generateHistogram();

int getDegree( LinkedList *connections ) {
    int counter = 0;
    Node *current = connections->head;

    while ( current != NULL) {
        if ( current->value >= 0 ) {
            counter++;
        }
        current = current->next;

    }

    return counter;
}

void connectNodes( LinkedList *network[], int node1, int node2 ) {
    append( network[node1], node2 );
    append( network[node2], node1 );

    return;
}

LinkedList **createNetwork( int numNodes ) {
    LinkedList *network[numNodes];
    int i;

    for ( i = 0; i < numNodes; i++ ) {
        network[i] = newList();
    }

    //Fully connect 3 nodes, then begin algorithm
    connectNodes( network, 0, 1 );
    connectNodes( network, 0, 2 );
    connectNodes( network, 1, 2 );

    return network;
}

/**
 * Currently working on having createNetwork return a network
 * as a list of LinkedLists.
 */
int main( int argc, char **argv ) {
    int i;
    printf( "testing\n" );
    LinkedList *network = *createNetwork( 3 );

    for ( i = 0; i < 3; i++ ) {
        printf( "%d\n", getDegree( &network[i] ));
    }


    //for(i = 0; i < 3; i++) {
    //    	delete(network[i]);
    //}
}