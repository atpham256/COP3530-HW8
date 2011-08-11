// Name: Anthony T. Pham
// UF ID: 5038-1111
// Discussion section #: 1079

#ifndef HW8_H
#define HW8_H

#include <iostream>
#include <utility>
#include <queue>
#include <stack>
#include "vertex.h"

using namespace std;
template<class T>
class undirectedGraph
{
	public:
		// constructors
		undirectedGraph()
		{

		}

		// methods
		int* connectedComponents();
		pair<int,int> *bfSpanningTree(int theVertex);
		bool findCycle();
		void cycleDFS(bool &cycle, int v, int* &visited, int* &parent);

		// destructor
		~undirectedGraph()
		{
			// free memory at each index
			for(int i = 0; i < numVertices + 1; i++)
			{
				//if( adjacencyList[i] !=0  )
				delete [] adjacencyList[i];
			}
			// free the rest of the array
			//delete [] adjacencyList;
		}

		// data members
		int numEdges, numVertices;
		vertex<T> **adjacencyList;
};

// connectedComponents method
// finds the connected components of the undirected graph
template<class T>
int* undirectedGraph<T>::connectedComponents()
{
	// variable to tell which component we are currently in
	int componentNum = 0;
	// create the array to tell which connected component each vertex is in
	// initialize to -1
	int *connectedComponentsArray = new int[numVertices + 1];
	for(int i = 0; i < (numVertices + 1); i++)
	{
		connectedComponentsArray[i] = -1;
	}
	// create an array to tell if we've visited a vertex already and initialize it to false
	// visited[i] tells whether we have already visited vertex i
	bool *visited = new bool[numVertices + 1];
	for(int i = 0; i < (numVertices + 1); i++)
	{
		visited[i] = false;
	}
	// create queue to do breadth-first search (BFS)
	queue<int> q;

	//do BFS for all components
	for(int i = 1; i < (numVertices + 1); i++)
	{
		// if connectedComponentsArray[i] = -1 then we have found a new component
		if(connectedComponentsArray[i] == -1)
		{
			// increment componentNum to reflect new component
			componentNum++;
			// do BFS
			q.push(i);
			visited[i] = true;
			connectedComponentsArray[i] = componentNum;
			while(!q.empty())
			{
				int v = q.front();
				q.pop();
				// push in all unvisited adjacent vertices into the queue
				for(vertex<T>* u = adjacencyList[v]; u != NULL; u = u->next)
				{
					if(!visited[u->vertexLabel])
					{
						// mark that the vertex has now been visited
						visited[u->vertexLabel] = true;
						// push the vertex into the queue
						q.push(u->vertexLabel);
						// mark which component the vertex belongs to
						connectedComponentsArray[u->vertexLabel] = componentNum;
					}
				}
			}
		}
	}
	// free memory
	delete [] visited;
	// return array telling which vertices belong to which connected components
	return connectedComponentsArray;
}

// bfSpanningTree method
// finds the spanning tree of a connected undirected graph
template<class T>
pair<int,int>* undirectedGraph<T>::bfSpanningTree(int theVertex)
{
	// check if the undirected graph is a CONNECTED undirected graph
	int* connectedComp = this->connectedComponents();
	bool connectedUndirectedGraph = true;
	for(int i = 1; i < numVertices + 1; i++)
	{
		if(connectedComp[i] > 1)
		{
			connectedUndirectedGraph = false;
		}
	}
	// if the undirected graph is a connected undirected graph then search for spanning tree
	if(connectedUndirectedGraph)
	{
		// create array to hold the edges of the BFS spanning tree
		pair<int,int> *edges = new pair<int,int>[numVertices - 1];
		// index for edges array
		int index = 0;
		// create an array to tell if we've visited a vertex already and initialize it to false
		// visited[i] tells whether we have already visited vertex i
		bool *visited = new bool[numVertices + 1];
		for(int i = 0; i < (numVertices + 1); i++)
		{
			visited[i] = false;
		}
		// create queue to do breadth-first search (BFS)
		queue<int> q;
		//do BFS
		q.push(theVertex);
		visited[theVertex] = true;
		while(!q.empty())
		{
			// pop out already visited vertex
			int v = q.front();
			q.pop();
			// push in all unvisited adjacent vertices into the queue
			for(vertex<T>* u = adjacencyList[v]; u != NULL; u = u->next)
			{
				if(!visited[u->vertexLabel])
				{
					// mark that the vertex has now been visited
					visited[u->vertexLabel] = true;
					// push the vertex into the queue
					q.push(u->vertexLabel);
					// add edge to the edges array
					pair<int,int> edge = pair<int,int>(v, u->vertexLabel);
					edges[index] = edge;
					index++;
				}
			}
		}
		// free memory
		delete [] visited;
		// return array of edges
		return edges;
	}
	else
	{
		// return NULL if the graph is not a connected undirected graph
		return NULL;
	}

}

// findCycle method
// tells whether the undirected graph has a cycle in it or not
template<class T>
bool undirectedGraph<T>::findCycle()
{
	// variable to hold result
	bool cycle = false;
	// stack for the depth first search (DFS)
	stack<int> mystack;
	// array to mark if we visited a vertex or not
	// 0 = unvisited
	// 1 = visited
	// 2 = visited vertex that has visited all of it's adjacent vertices
	int *visited = new int[numVertices + 1];
	for(int i = 0; i < numVertices + 1; i++)
	{
		visited[i] = 0;
	}
	// array to mark the node we came from to get to vertex i
	int *parent = new int[numVertices + 1];
	for(int i = 0; i < numVertices + 1; i++)
	{
		parent[i] = 0;
	}

	// do a DFS for each component of the array
	for(int i = 1; i < numVertices + 1; i++)
	{
		// if we haven't visited a vertex, we found a new component
		if(visited[i] == 0)
		{
			this->cycleDFS(cycle, i, visited, parent);
		}
	}
	// free memory
	delete [] visited;
	delete [] parent;
	// return if there is a cycle or not
	return cycle;
}

// cycleDFS method - helps findCycle method
template<class T>
void undirectedGraph<T>::cycleDFS(bool &cycle, int v, int* &visited, int* &parent)
{
	// do DFS
	// mark that we have visited the vertex
	visited[v] = 1;
	// do a DFS for each of the adjacent vertices
	for(vertex<T> *u = adjacencyList[v]; u != NULL; u = u->next)
	{
		if(visited[u->vertexLabel] == 0)
		{
			// update parent of adjacent vertex
			parent[u->vertexLabel] = v;
			// do a DFS on the adjacent vertex
			this->cycleDFS(cycle, u->vertexLabel, visited, parent);
		}
		// if we have visited the adjacent vertex before and this vertex is not
		// the vertex we came from previously, then there is a cycle
		if(visited[u->vertexLabel] == 1 && parent[v] != u->vertexLabel)
		{
			cycle = true;
			break;
		}
	}
	// after the for-loop, vertex v will have visited all of its adjacent vertices
	visited[v] = 2;
}
#endif
