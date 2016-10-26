#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
int degreeSum( LinkedList *network, int numNodes );

/**
 * Return the value of the highest degree in the network
 */
int maxDegree( LinkedList *network, int numNodes );

/**
 * Return the number of nodes in the network with the given degree
 */
int numWithDegree( LinkedList *network, int numNodes, int degree );

/**
 * Updates the probability of a node connection as nodes are added
 */
double updateProbabilities( LinkedList *network, int numNodes, int node );

/**
 * Save the network to a .csv file
 */
void storeNetwork( char filename[], LinkedList *network, int numNodes );

/**
 * Produces a histogram based on network size formatted as:
 * "Node ID, connection1, connection2, ..."
 */
void generateHistogram( LinkedList *network, int numNodes, char *filename );

int getDegree( LinkedList *connections ) {
    int counter = 0;
    Node *current = connections->head->next;

    while ( current->node != NULL) {
        counter++;
        current = current->next;
    }
    return counter;
}

int degreeSum( LinkedList *network, int numNodes ) {
    int i;
    int total = 0;
    for ( i = 0; i < numNodes; i++ ) {
        total += getDegree( find( network, i )->neighbors );
    }
    return total;
}

int maxDegree( LinkedList *network, int numNodes ) {
    int i;
    int highest = 0;
    for ( i = 0; i < numNodes; i++ ) {
        if ( highest < getDegree( find( network, i )->neighbors )) {
            highest = getDegree( find( network, i )->neighbors );
        }
    }
    return highest;
}

int numWithDegree( LinkedList *network, int numNodes, int degree ) {
    int i;
    int total = 0;
    for ( i = 0; i < numNodes; i++ ) {
        if ( degree == getDegree( find( network, i )->neighbors )) {
            total++;
        }
    }
    return total;
}

double updateProbabilities( LinkedList *network, int numNodes, int node ) {
    double degree = getDegree( find( network, node )->neighbors );
    double allDegrees = degreeSum( network, numNodes );
    return degree / allDegrees;
}


LinkedList *createNetwork( int numNodes ) {
    int i;
    int connect = 0;
    int next = 3;
    srand((unsigned int) time(NULL));
    double chance;
    double prev;
    double probability[numNodes];
    double percent[numNodes];
    LinkedList *network = newList();

    //Create the LinkedList describing the network; fill with nodes
    for ( i = 0; i < numNodes; i++ ) {
        GraphNode *node = newGraphNode( i );
        append( network, node );
    }

    //Set up base case for generating the topology: 3 fully connected nodes
    link( find( network, 0 ), find( network, 1 ));
    link( find( network, 0 ), find( network, 2 ));
    link( find( network, 1 ), find( network, 2 ));

    //Initialize arrays
    for ( i = 0; i < numNodes; i++ ) {
        probability[i] = 0.0;
        percent[i] = 0.0;
    }


    //Algorithmically connect new nodes until network is complete
    while ( next < numNodes ) {
        printf( "Connecting node %d\n", next );
        connect = 0;
        prev = 0;
        chance = rand() / (RAND_MAX + 1.0);
        for(i = 0; i < numNodes; i++) {
        	if((chance >= prev) &&
        		(chance < (updateProbabilities(network, numNodes, i) + prev))) {
        		connect = i;
        		break;
        	} else {
        		prev += updateProbabilities(network, numNodes, i);
        	}
        }
        link(find(network, next), find(network, connect));
        next++;
    }


    return network;
}

void generateHistogram( LinkedList *network, int numNodes, char *filename ) {
    int i;
    int j;
    int count;
    int range = maxDegree( network, numNodes );
    FILE *output;
    char name[64];
    strcpy(name, filename);
    strcat(name, ".csv");
    output = fopen( name, "w" );
    for ( i = 1; i <= range; i++ ) {
        fprintf( output, "%d,", i );
        fprintf( output, "%d", numWithDegree( network, numNodes, i ));
        count = 0;
        for ( j = 0; j < numNodes; j++ ) {
            if ( getDegree( find( network, j )->neighbors ) == i ) {
                fprintf( output, ", %d", j );
            }
        }
        fprintf( output, "\n" );
    }
    fclose( output );
}

void storeNetwork( char filename[], LinkedList *network, int numNodes ) {
    int i;
    FILE *output;
    char name[64];
    Node *current;

    strcpy( name, filename );
    strcat( name, ".csv" );
    output = fopen( name, "w" );

    for ( i = 0; i < numNodes; i++ ) {
        current = find( network, i )->neighbors->head->next;
        fprintf( output, "%d", i );

        while ( current->node != NULL) {
            fprintf( output, ", %d", current->node->value );
            current = current->next;
        }
        fprintf( output, "\n" );

    }

    fclose( output );
}

/**
 * Checked that degreeSum works properly; should output 6
 */
int main( int argc, char **argv ) {
    //Check validity of argument count
    if ( argc != 3 && argc != 5 ) {
        printf( "Illegal Arguments, please check inputs\n" );
        return 1;
    }
    int histogram = argc == 5 ? 1 : 0;
    printf( "Generating...\n" );
    int numNodes = atoi( argv[1] );
    LinkedList *network = createNetwork( numNodes );
    storeNetwork( argv[2], network, numNodes );
    printf( "\n" );
    printf( argv[2] );
    printf( " generated\n" );
    if ( histogram ) {
        generateHistogram( network, numNodes, argv[4] );
        printf( argv[4] );
        printf( " generated as histogram\n" );
    }
    deleteList( network );
}
