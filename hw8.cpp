// Name: Anthony T. Pham
// UF ID: 5038-1111
// Discussion section #: 1079

#include<iostream>
#include<fstream>
#include "hw8.h"

using namespace std;

// overload >> operator
template<class T>
istream& operator>>(istream& in, undirectedGraph<T>& x)
{
	int temp, v1, v2;
	// read in the number of vertices
	in >> x.numVertices;
	// read in vertex labels
	for(int i = 0; i < x.numVertices; i++)
	{
		in >> temp;
	}
	// create space for adjacency list
	x.adjacencyList = new vertex<T>*[x.numVertices + 1];
	// initialize pointers to null
	for(int i = 0; i < x.numVertices + 1; i++)
	{
		x.adjacencyList[i] = NULL;
	}
	// read in the number of edges
	in >> x.numEdges;
	// read in the edges into the adjacency list
	for(int i = 0; i < x.numEdges; i++)
	{
		in >> v1 >> v2;
		// add in v2 as adjacent vertex of v1
		if(x.adjacencyList[v1] == NULL)
		{
			x.adjacencyList[v1] = new vertex<T>(v2);
		}
		else
		{
			vertex<T> *vChain = x.adjacencyList[v1];
			//vChain->element = x.adjacencyList[v1]->element;
			//vChain->next = x.adjacencyList[v1]->next;
			vertex<T> *newV = new vertex<T>(v2, vChain);
			x.adjacencyList[v1] = newV;
			//x.adjacencyList[v1]->element = newV->element;
			//x.adjacencyList[v1]->next = newV->next;
		}

		// add in v1 as adjacent vertex of v2
		if(x.adjacencyList[v2] == NULL)
		{
			x.adjacencyList[v2] = new vertex<T>(v1);
		}
		else
		{
			vertex<T> *vChain = x.adjacencyList[v2];
			vertex<T> *newV = new vertex<T>(v1, vChain);
			x.adjacencyList[v2] = newV;
		}
	}

	return in;
}

// overload << operator
template<class T>
ostream& operator<<(ostream& out, undirectedGraph<T>& x)
{
	// print connected components information
	int* connectedComp = x.connectedComponents();
	int numComponents = 0;
	for(int i = 0; i < x.numVertices + 1; i++)
	{
		if(numComponents < connectedComp[i])
		{
			numComponents = connectedComp[i];
		}
	}
	out << "# Connected Components = " << numComponents << endl;
	for(int i = 1; i < numComponents + 1; i++)
	{
		for(int j = 0; j < x.numVertices + 1; j++)
		{
			if(connectedComp[j] == i)
			{
				out << j << " ";
			}
		}
		out << endl;
	}
	out << endl;

	// print out bfSpanningTree information
	out << "edges of spanning tree" << endl;
	pair<int,int> *ST = x.bfSpanningTree(1);
	if(ST == NULL)
	{
		out << "no spanning tree" << endl;
	}
	else
	{
		for(int i = 0; i < x.numVertices - 1; i++)
		{
			out << ST[i].first << " " << ST[i].second;
			out << endl;
		}
	}
	out << endl;

	if(x.findCycle())
	{
		out << "Cycle: Yes";
	}
	else
	{
		out << "Cycle: No";
	}

	// free memory
	delete [] connectedComp;
	delete [] ST;

	return out;
}
int main()
{
	// variable declarations and file streams
	undirectedGraph<int> x;
	ifstream inFile;
	inFile.open("hw8.in");
	ofstream outFile("hw8.out");

	// read in undirected graph
	inFile >> x;

	// output undirected graph to file
	outFile << x;

	// free memory
	x.~undirectedGraph();

	cout << "Writing to file complete." << endl;

	// close file streams
	inFile.close();
	outFile.close();

	return 0;
}
