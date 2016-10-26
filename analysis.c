#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Parses the CSV file to a graph
 * @param filename : the name of the CSV file to be parsed, in the format of each line representing a node and its neighbors
 * the first cell being the node number of the node, and the remaining cells its neighbors
 * @return : Array of GraphNodes that are connected, the first node holds information on the number of nodes
 */
GraphNode **parseCsv( char *filename ) {
    int nodeCount = 0;
    //Get number of nodes
    FILE *csvFile = fopen( filename, "r" );
    while ( !feof( csvFile )) {
        if ( fgetc( csvFile ) == '\n' )
            nodeCount++;
    }
    rewind( csvFile );
    //Create node array, first element value is the NUMBER OF NODES
    GraphNode **nodes = (GraphNode **) calloc( sizeof( GraphNode * ), (size_t) nodeCount + 1 );
    nodes[0] = newGraphNode( nodeCount );
    for ( int i = 1; i < nodeCount + 1; i++ ) nodes[i] = NULL;
    fseek( csvFile, 0, SEEK_END );
    long fileLength = ftell( csvFile );
    fseek( csvFile, 0, SEEK_SET );
    char *csvContents = calloc( sizeof( char ), (size_t) fileLength );
    fread( csvContents, 1, (size_t) fileLength, csvFile );
    fclose( csvFile );
    //Allocates space
    char **csvLines = (char **) calloc( sizeof( char * ), (size_t) nodeCount );
    int count = 0;
    for ( char *line = strtok( csvContents, "\n" ); line != NULL; line = strtok(NULL, "\n" )) {
        csvLines[count] = (char *) calloc( sizeof( char ), (size_t) strlen( line ) + 2 );
        strcpy( csvLines[count], line );
        count++;
    }
    int nodeIndex = 1;
    //Parses each line of csv
    for ( count = 0; count < nodeCount; count++ ) {
        int neighborIndex = 0;
        int curNode = -1;
        for ( char *num = strtok( csvLines[count], "," ); num; num = strtok(NULL, "," )) {
            nodeIndex = atoi( num ) + 1;
            if ( !neighborIndex ) {
                if ( !nodes[nodeIndex] ) {
                    nodes[nodeIndex] = newGraphNode( atoi( num ) + 1 );
                }
                curNode = atoi( num ) + 1;
            } else {
                int targetNode = atoi( num ) + 1;
                if ( !nodes[targetNode] ) {
                    nodes[targetNode] = newGraphNode( targetNode );
                }
                link( nodes[curNode], nodes[targetNode] );
            }
            neighborIndex++;
        }
    }
    //Free memory used for parsing CSV file
    free( csvLines );
    free( csvContents );
    return nodes;
}

/**
 * Parses a node of the existing graph, returns a linked list of the nodes farthest away from
 * the current node
 * @param nodes : the graph
 * @param index : index of the current node on the graph
 * @return : the linked list described above, the head of the linkedlist holds the distance value,
 * the tail of the linkedlist holds the node number of the current node
 */
LinkedList *parseDistance( GraphNode **nodes, int index ) {
    //Extract information of the graph
    int nodeCount = nodes[0]->value;
    //distances array holds array of nodes with the index as the distance to the origin node
    LinkedList **distances = (LinkedList **) calloc( sizeof( LinkedList * ), (size_t) nodeCount );
    for ( int i = 0; i < nodeCount; i++ ) {
        distances[i] = NULL;
    }
    //The node itself is appended to distances[0]
    distances[0] = newList();
    append( distances[0], newGraphNode( nodes[index]->value ));
    //If a node is marked, it has been visited
    nodes[index]->marked = 1;
    int i = 1;
    //Expand the distance by 1 for each iteration, until there are no more nodes
    for ( ; i < nodeCount; i++ ) {
        if ( !distances[i - 1] || isEmpty( distances[i - 1] )) {
            break;
        } else {
            distances[i] = newList();
        }
        Node *curNode = distances[i - 1]->head->next;
        while ( curNode != distances[i - 1]->tail ) {
            LinkedList *neighbors = nodes[curNode->node->value]->neighbors;
            Node *neighbor = neighbors->head->next;
            while ( neighbor != neighbors->tail ) {
                GraphNode *neighborGraphNode = neighbor->node;
                //Add unvisited neighbors to the distance list
                if ( !neighborGraphNode->marked ) {
                    append( distances[i], newGraphNode( neighborGraphNode->value ));
                    neighborGraphNode->marked = 1;
                }
                neighbor = neighbor->next;
            }
            curNode = curNode->next;
        }
    }
    //Copy the item (LinkedList) at the maximum valid index of the distances array, because distances
    //array is going to be freed
    LinkedList *ret = newList();
    ret->head->node = newGraphNode( i - 2 );
    ret->tail->node = newGraphNode( nodes[index]->value );
    for ( Node *curNode = distances[i - 2]->head->next; curNode != distances[i - 2]->tail; curNode = curNode->next ) {
        append( ret, curNode->node );
    }
    //Mark all nodes as unvisited for future calls
    for ( int j = 1; j <= nodeCount; j++ ) {
        nodes[j]->marked = 0;
    }
    //Free all allocated space, except for the return value
    for ( int j = 0; j < nodeCount; j++ ) {
        if ( distances[j] ) {
            deleteList( distances[j] );
        }
    }
    free( distances );

    return ret;
}

int main( int argc, char **argv ) {
    //Check if argument count is valid
    if ( argc != 2 && argc != 4 ) {
        printf( "Illegal Arguments, please check input\n" );
        return 1;
    }
    int outputToFile = argc == 4 ? 1 : 0;
    //Parse CSV file specified in the arguments
    GraphNode **nodes = parseCsv( argv[1] );
    int nodeCount = nodes[0]->value;
    int diameter = 0;
    char output[262144] = "";
    for ( int i = 1; i <= nodeCount; i++ ) {
        char line[65536] = "";
        //Build the output string line by line
        LinkedList *curResult = parseDistance( nodes, i );
        sprintf( line, "Node %d: Maximum Distance - %d, Nodes at Max Distance:", curResult->tail->node->value - 1,
                 curResult->head->node->value );
        for ( Node *curNode = curResult->head->next;
              curNode != curResult->tail; curNode = curNode->next ) {
            char node[8] = "";
            sprintf( node, " %d", curNode->node->value - 1 );
            strcat( line, node );
        }
        strcat( line, "\n" );
        strcat( output, line );
        int curMax = curResult->head->node->value;
        //Diameter is the maximum of all max distances on each graph node
        diameter = curMax > diameter ? curMax : diameter;
        //Free allocated space
        for ( Node *curNode = curResult->head;
              curNode; curNode = curNode->next ) {
            delete( curNode->node );
        }
        deleteList( curResult );
    }
    //Free allocated space
    for ( int i = 0; i <= nodeCount; i++ ) {
        delete( nodes[i] );
    }
    free( nodes );
    char finalOutput[262144] = "";
    //Append diameter to the output string
    sprintf( finalOutput, "Diameter: %d\n", diameter );
    strcat( finalOutput, output );
    //Print to console or output to file depending on the arguments passed in
    if ( !outputToFile ) {
        printf( finalOutput );
    } else {
        FILE *outputFile = fopen( argv[3], "w" );
        fputs( finalOutput, outputFile );
        fclose( outputFile );
        char outputLine[50] = "";
        sprintf( outputLine, "Output " );
        strcat( outputLine, argv[3] );
        strcat( outputLine, " generated\n" );
        printf( outputLine );
    }
    return 0;
}