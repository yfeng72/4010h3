#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

/**
 * Instantiates and builds the network
 */
//LinkedList **createNetwork( int numNodes );

/**
 * Returns the number of connections a node has.
 */
int getDegree( LinkedList *connections );

/**
 * Return the sum of all degree nodes.
 */
int degreeSum( LinkedList* network, int numNodes);

/**
 * Save the network to a .csv file
 */
//void storeNetwork( char filename[] );

/**
 * Produces a histogram based on network size formatted as:
 * "Node ID, connection1, connection2, ..."
 */
//void generateHistogram();

int getDegree( LinkedList *connections ) {
    int counter = 0;
    Node* current = connections->head->next;

    while ( current->node != NULL) {
    	counter++;
    	current = current->next;
    }
    return counter;
}

int degreeSum( LinkedList* network, int numNodes) {
	int i;
	int total = 0;
	for(i = 0; i < numNodes; i++) {
		total += getDegree(find(network, i)->neighbors);
	}
	return total;
}


LinkedList* createNetwork( int numNodes ) {
	int i;
	LinkedList* network = newList();

	//Create the LinkedList describing the network; fill with nodes
	for(i = 0; i < numNodes; i++) {
		GraphNode* node = newGraphNode(i);
		append(network, node);
	}

	//Set up base case for generating the topology: 3 fully connected nodes
	link(find(network, 0), find(network, 1));
	link(find(network, 0), find(network, 2));
	link(find(network, 1), find(network, 2));



	return network;
}

/**
 * Checked that degreeSum works properly; should output 6
 */
int main( int argc, char **argv ) {
    printf( "testing\n" );
    LinkedList* network = createNetwork(5);
    printf("%d\n", degreeSum(network, 5));
    deleteList(network);

}
