#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GraphNode **parseCsv( char *filename ) {
    int nodeCount = 1;
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

    char **csvLines = (char **) calloc( sizeof( char * ), (size_t) nodeCount );\
    int count = 0;
    for ( char *line = strtok( csvContents, "\n" ); line != NULL; line = strtok(NULL, "\n" )) {
        csvLines[count] = (char *) calloc( sizeof( char ), (size_t) strlen( line ) + 2 );
        strcpy( csvLines[count], line );
        count++;
    }
    int nodeIndex = 1;
    for ( count = 0; count < nodeCount; count++ ) {
        int neighborIndex = 0;
        int curNode = -1;
        for ( char *num = strtok( csvLines[count], "," ); num; num = strtok(NULL, "," )) {
            if ( !neighborIndex ) {
                if ( !nodes[nodeIndex] ) {
                    nodes[nodeIndex] = newGraphNode( atoi( num ));
                }
                curNode = atoi( num );
            }
            else {
                int targetNode = atoi( num );
                if ( !nodes[targetNode] ) {
                    nodes[targetNode] = newGraphNode( targetNode );
                }
                link( nodes[curNode], nodes[targetNode] );
            }
            neighborIndex++;
        }
    }
    for ( int i = 0; i < nodeCount; i++ ) free( csvLines[i] );
    free( csvLines );
    free( csvContents );
    return nodes;
}

LinkedList *parseDistance( GraphNode **nodes, int index ) {
    int nodeCount = nodes[0]->value;
    LinkedList **distances = (LinkedList **) calloc( sizeof( LinkedList * ), (size_t) nodeCount );
    distances[0] = newList();
    append( distances[0], newGraphNode( nodes[index]->value ));
    nodes[index]->marked = 1;
    int i = 1;
    for ( ; i < nodeCount; i++ ) {
        if ( !distances[i - 1] || isEmpty( distances[i - 1] )) {
            break;
        }
        else {
            distances[i] = newList();
        }
        for ( Node *curNode = distances[i - 1]->head->next;
              curNode != distances[i - 1]->tail; curNode = curNode->next ) {
            LinkedList *neighbors = nodes[curNode->node->value]->neighbors;
            for ( Node *neighbor = neighbors->head->next; neighbor != neighbors->tail; neighbor = neighbor->next ) {
                GraphNode *neighborGraphNode = neighbor->node;
                if ( !neighborGraphNode->marked ) {
                    append( distances[i], newGraphNode( neighborGraphNode->value ));
                    neighborGraphNode->marked = 1;
                }
            }
        }
    }
    LinkedList *ret = newList();
    ret->head->node = newGraphNode( i - 2 );
    for ( Node *curNode = distances[i - 2]->head->next; curNode != distances[i - 2]->tail; curNode = curNode->next ) {
        append( ret, curNode->node );
    }
    for ( int j = 1; j <= nodeCount; j++ ) {
        nodes[j]->marked = 0;
    }
    for ( int j = 0; j < nodeCount; j++ ) {
        if ( distances[j] ) {
            for ( Node *curNode = distances[j]->head->next;
                  curNode != distances[j]->tail; curNode = curNode->next ) {
                delete( curNode->node );
            }
            deleteList( distances[j] );
        }
    }
    return ret;
}

int main( int argc, char **argv ) {
    GraphNode **nodes = parseCsv( "a.csv" );

    for ( int i = 1; i < 4; i++ ) {
        GraphNode *curNode = nodes[i];
        printf( "%d\n", curNode->value );
    }
    for ( int i = 1; i < 4; i++ ) {
        LinkedList *result = parseDistance( nodes, i );
        printf( "\n" );
        printf( "Dist: %d\n", result->head->node->value );
        for ( Node *curNode = result->head->next;
              curNode != result->tail; curNode = curNode->next ) {
            printf( "%d\n", curNode->node->value );
        }
    }
}