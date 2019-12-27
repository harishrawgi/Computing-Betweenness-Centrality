a.out : main.o Graph.o GraphHelper.o AdjList.o Queue.o Stack.o PredecessorList.o AdjacencyMatrix.o
	gcc -O3 main.o Graph.o GraphHelper.o AdjList.o Queue.o Stack.o PredecessorList.o AdjacencyMatrix.o

main.o: main.c Graph.h
	gcc -c -O3 main.c

Graph.o: Graph.c Graph.h GraphHelper.h
	gcc -c -O3 Graph.c

GraphHelper.o: GraphHelper.c GraphHelper.h Stack.h Queue.h PredecessorList.h Graph.h AdjacencyMatrix.h
	gcc -c -O3 GraphHelper.c

AdjList.o: AdjList.c AdjList.h
	gcc -c -O3 AdjList.c

AdjacencyMatrix.o: AdjacencyMatrix.c AdjacencyMatrix.h
	gcc -c -O3 AdjacencyMatrix.c

Stack.o: Stack.c Stack.h
	gcc -c -O3 Stack.c

Queue.o: Queue.c Queue.h
	gcc -c -O3 Queue.c

PredecessorList.o: PredecessorList.c PredecessorList.h GraphHelper.h
	gcc -c -O3 PredecessorList.c
