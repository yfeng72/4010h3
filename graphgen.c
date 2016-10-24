#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

/**
 * Instantiates and builds the network
 */
LinkedList *createNetwork( int numNodes );

/**
 * Returns the number of connections a node has.
 */
int getDegree( LinkedList *connections );

/**
 * Return the sum of all degree nodes.
 */
int degreeSum( LinkedList* network, int numNodes);

/**
 * Updates the probability of a node connection as nodes are added
 */
double updateProbabilities(LinkedList* network, int numNodes, int node);

/**
 * Save the network to a .csv file
 */
void storeNetwork(char filename[], LinkedList* network, int numNodes);

/**
 * Produces a histogram based on network size formatted as:
 * "Node ID, connection1, connection2, ..."
 */
void generateHistogram();

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

double updateProbabilities(LinkedList* network, int numNodes, int node) {
	double degree = getDegree(find(network, node)->neighbors);
	double allDegrees = degreeSum(network, numNodes);
	return degree / allDegrees;
}


LinkedList* createNetwork( int numNodes ) {
	int i;
	int connect = 0;
	int next = 3;
	srand(time(NULL));
	double chance;
	double probability[numNodes];
	double percent[numNodes];
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

	//Initialize arrays
	for(i = 0; i < numNodes; i++) {
		probability[i] = 0.0;
		percent[i] = 0.0;
	}


	//Algorithmically connect new nodes until network is complete
	while(next < numNodes) {
		connect = 0;
		chance = rand() / (RAND_MAX + 1.0);
		for(i = 0; i < numNodes; i++) {
			probability[i] = updateProbabilities(network, numNodes, i);
			if(i == 0) {
				percent[i] = 0.0;
			} else if(probability[i] == 0){
				percent[i] = 1.0;
			} else {
				percent[i] = percent[i - 1] + probability[i];
			}
		}

		for(i = 0; i < numNodes; i++) {
			if(percent[i] == 1.0){
				break;
			} else if(chance > percent[i + 1]) {
				connect = i;
			}
		}
		link(find(network, next), find(network, connect));
		next++;
	}


	return network;
}

void generateHistogram(LinkedList* network, int numNodes) {

}

void storeNetwork(char filename[], LinkedList* network, int numNodes) {
	int i;
	FILE *output;
	char name[64];
	Node* current;

	strcpy(name, filename);
	strcat(name, ".csv");
	output = fopen(name, "w");

	for(i = 0; i < numNodes; i++) {
		current = find(network, i)->neighbors->head->next;
		fprintf(output, "%d", i);

		while ( current->node != NULL) {
	    	fprintf(output, ", %d", current->node->value);
	    	current = current->next;
		}
		fprintf(output, "\n");

	}

	fclose(output);
}

/**
 * Checked that degreeSum works properly; should output 6
 */
int main( int argc, char **argv ) {
	int i;
    printf( "testing\n" );
    LinkedList* network = createNetwork(100);
    for(i = 0; i < 100; i++){
    	printf("%d, %d\n", i, getDegree(find(network, i)->neighbors));
    }
    storeNetwork("hello", network, 100);
    deleteList(network);
}
