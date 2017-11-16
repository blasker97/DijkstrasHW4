/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Brian L
 *
 * Created on November 13, 2017, 5:02 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "input_error.h"
#include <limits.h>
#include "heap.h"


struct node
{
    int vertex;
    int distance;
    int weight;
    struct node* next;
    int dest;
};

struct Graph
{
    int numVertices;
    struct node** adjLists;
};


struct Graph* parse_getline(FILE* fp, struct Graph* unGraph);
struct Graph* createGraph(int vertices);
struct node* createNode(int v, int w);
void addEdge(struct Graph* graph, int src, int dest, int weight);
void printGraph(struct Graph* graph);
int* dijkstra(struct Graph*, int src);
void printArr(int dist[], int n);




int main(int argc, char** argv) {
    
    if (argc!=3)
        exit(INCORRECT_NUMBER_OF_COMMAND_LINE_ARGUMENTS);
    
    FILE* fp = fopen(argv[1],"r");
    
    if(!fp)
        exit(INPUT_FILE_FAILED_TO_OPEN);
    
    if(fgetc(fp) == EOF)
        exit(PARSING_ERROR_EMPTY_INPUT_FILE);
    fseek(fp, 0, SEEK_SET);
    
    struct Graph* unGraph;
    int* dist;
    int max = -1;
    
    unGraph = parse_getline(fp, unGraph);
   
    
    
    //printGraph(unGraph);
    
    dist = dijkstra(unGraph, 0);
  
    
    if (fclose(fp) == EOF)
	exit(INPUT_FILE_FAILED_TO_CLOSE);
    
   
    
    fp = fopen(argv[2],"w");
    
    if(!fp)
        exit(OUTPUT_FILE_FAILED_TO_OPEN);
    
    for (int i = 0; i < unGraph->numVertices; i++){
        if(dist[i] == INT_MAX){
            fprintf(fp,"%d\n", max);
        }
        else{
            fprintf(fp,"%d\n",dist[i]);
        }
    }
    
    if (fclose(fp) == EOF)
        exit(OUTPUT_FILE_FAILED_TO_CLOSE);
   
    free(dist);
    
    for(int i = 0; i<unGraph->numVertices; i++){
        free(unGraph->adjLists[i]);
    }
    

    return (EXIT_SUCCESS);
}

struct Graph* parse_getline(FILE* fp, struct Graph* unGraph) {
	char* line = NULL;
        int numOfSrc;
        int numOfDest;
        int weight;
	size_t nbytes = 0;
        int i=0, linelen=0, j=0, numOfVertex = 0 , k =0;
	while ((linelen=getline(&line, &nbytes, fp)) != -1) {
		line[linelen-1] = '\0'; //removing the newline and adding the NULL character
                      
                if(j == 0){
                    for(i=0; i<linelen-1; i++){
                        if(isdigit(line[i])==0){
                            exit(PARSING_ERROR_INVALID_FORMAT);
                        }
                        else{
                            if(k==0){
                            numOfVertex += line[i] - '0';
                            k++;
                            }
                            else{
                                numOfVertex *= 10;
                                numOfVertex+= line[i] - '0';
                            }
                        }
                    }	
                    unGraph = createGraph(numOfVertex);  
                    j++;
                }
                else{
                    
                    numOfSrc = 0;
                    numOfDest = 0;
                    weight = 0;
                    
                    if(line[0] != '('  || line[linelen-2] != ')'){
                        exit(PARSING_ERROR_INVALID_FORMAT);
                    }
                    
                    for(i = 0; i<linelen - 1; i++){
                        if(line[i] == ','){
                            break;
                        }
                        if(i == linelen - 2){
                            exit(PARSING_ERROR_INVALID_FORMAT);
                        }
                    }
                    
                     
                    
                    i = 1;
                    k = 0;
                    while(line[i]!=','){
                        if(k==0){
                            numOfSrc += line[i] - '0';
                            k++;
                        }
                        else{
                            numOfSrc *= 10;
                            numOfSrc += line[i] - '0';
                        }
                        i++;
                    }
                    i++;
                    k = 0;
                    while(line[i]!=','){
                        if(k==0){
                            numOfDest += line[i] - '0';
                            k++;
                        }
                        else{
                            numOfDest *= 10;
                            numOfDest += line[i] - '0';
                        }
                        i++;
                    }
                    i++;
                    k = 0;
                    while(line[i]!=')'){
                        if(k==0){
                            weight += line[i] - '0';
                            k++;
                        }
                        else{
                            weight *= 10;
                            weight += line[i] - '0';    
                        }
                        i++;
                    }
                    
                    
                    if(numOfSrc == 0 || numOfDest == 0 || weight == 0)
                        exit(PARSING_ERROR_INVALID_FORMAT);
                    
                    if(numOfSrc > numOfVertex || numOfDest > numOfVertex)
                        exit(INTEGER_IS_NOT_A_VERTEX);
                   
                    addEdge(unGraph,numOfSrc,numOfDest,weight);
                }
        
       	}

	free(line);
        return unGraph;
}

// creates node for linked list for the vertex sent
struct node* createNode(int v, int w)
{
    struct node* newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->next = NULL;
    return newNode;
}
 
//builds structure that is an array going down and linked list out for adjs vertexs
struct Graph* createGraph(int vertices)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
 
    graph->adjLists = malloc(vertices * sizeof(struct node*));
 
    int i;
    for (i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;
 
    return graph;
}
 
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    //takes adjlist location and subtracts by 1 so the 0 position is not skipped
    // Add edge from src to dest
    struct node* newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src-1];
    graph->adjLists[src-1] = newNode;
    
 
//    // Add edge from dest to src
//    newNode = createNode(src, weight);
//    newNode->next = graph->adjLists[dest-1];
//    graph->adjLists[dest-1] = newNode;
}
 
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->numVertices; v++)
    {
        struct node* temp = graph->adjLists[v];
        printf("\n Adjacency list of vertex %d\n ", v+1);
        while (temp)
        {
            printf("%d (weight = %d) -> ", temp->vertex, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

int* dijkstra(struct Graph* graph, int src)
{
        //get vertices and malloc an int pointer to hold all the cost to each destination
	int totalVertices = graph->numVertices;
	int* dist = malloc(sizeof(int) * totalVertices);

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(totalVertices);

	
	for (int i = 0; i < totalVertices; ++i)
	{
		dist[i] = INT_MAX; //set infinity to int max so it does not interfere with min heap
		minHeap->array[i] = newMinHeapNode(i, dist[i]);
		minHeap->pos[i] = i;
	}

	
	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	//set size equal to total number of vertices
	minHeap->size = totalVertices;

	while (!isEmpty(minHeap))
	{
		// Extract the vertex with minimum distance value
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v; // Store the extracted vertex number

		
		struct node* temp = graph->adjLists[u];
		while (temp != NULL)
		{
			int v = temp->vertex-1;

			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && 
                                          temp->weight + dist[u] < dist[v])
			{
				dist[v] = dist[u] + temp->weight;

				// update distance value in min heap also
				decreaseKey(minHeap, v, dist[v]);
			}
			temp = temp->next;
		}
	}
        //printArr(dist, V);
        return dist;
}

void printArr(int dist[], int n)
{
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; ++i)
		printf("%d \t\t %d\n", i, dist[i]);
}



