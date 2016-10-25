CX 4010 Project 3
10/28/16

# Authors

Yu Feng, Douglas Stewart

# Compilation and Run Instructions

To compile the GraphGen application:
gcc -o graphgen linkedList.c linkedList.h graphgen.c -std=c99 -lm

To compile the Analysis application:
gcc -o analysis linkedList.c linkedList.h analysis.c -std=c99 -lm

To run the graphgen application:
./graphgen <# of nodes> [-h <filename>]

To run the analysis application:
./analysis <CSV filename> [-o <output filename>]

