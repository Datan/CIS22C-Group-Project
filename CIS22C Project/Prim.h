#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "LinkedGraph.h"
#include "Vertex.h"
#include "Edge.h"
#include "LinkedStack.h"

using namespace std;

template <class LabelType>
class Prim : public LinkedGraph < LabelType >
{
	// NESTED CLASS: (needed because the Edge class didn't give both ends)
	template <class LabelType>
	class PrimEdge
	{
	private:
		LabelType startLabel;
		Edge<LabelType> edge;
		bool checked;

	public:
		PrimEdge(){}
		PrimEdge(LabelType start, Edge<LabelType> ed){
			startLabel = start; edge = ed; checked = false;
		}

		void setChecked(bool value){ checked = value; }
		LabelType getStart()const { return startLabel; }
		LabelType getEnd() const { return edge.getEndVertex(); }
		float getWeight() const { return edge.getWeight(); }
		bool isChecked() const { return checked; }

		bool operator<(const PrimEdge &right)
		{
			return this->edge.getWeight() < right.getWeight();
		}


	};

private:
	//Private member variables
	LinkedStack<PrimEdge<LabelType>> undoStack;
	vector<PrimEdge<LabelType>> minSpanTree;	
	vector<PrimEdge<LabelType>> orderedEdges;	//Probably should rename, since the edges of the graph we are given are not ordered... - Luke

	/* Most likely will use this in applyPrims, which are the neighbor edges of visited vertices
	because unlike Kruskal, Prim does not know the ordering of all the edges to begin with
	- Luke
	*/
	vector<PrimEdge<LabelType>> localOrderedEdges;

	//Private member functions
	void applyPrim();
	bool getLocalUnvisitedNeighbors(LabelType currVertex, vector<PrimEdge<LabelType>> &tempVect);
	void selectionSort(vector<PrimEdge<LabelType>> &array);
	bool notConnected(LabelType &end1, LabelType &end2);
	void writeVector(ostream &os, vector<PrimEdge<LabelType>> &vect);
	void writeLocalOrderedEdges(ostream &os);
	void findRightVertexAndVectEdge(vector<PrimEdge<LabelType>>& tempVect, LabelType &currVertex, vector<LabelType> &visitedVertexVect);

	//	bool disconnectVisitedVertex(Vertex<LabelType>& visitedVertex);

public:
//	Prim(){}
//	~Prim(){}
	void createMinSpanTree();
	void writeMinSpanTree(ostream &os);
	void writeOrderedEdges(ostream &os); // for debugging
	bool undo();
	bool add(LabelType start, LabelType end, float edgeWeight = 0);
	bool remove(LabelType start, LabelType end);
};

/*
template <class LabelType>
bool Prim::disconnectVisitedVertex(Vertex<LabelType>& visitedVertex)
{
Vertex<LabelType>* currVer;

if (visitedVertex.isVisited())
{
for (int i = 0; i < visitedVertex.getNumberOfNeighbors(); i++)
{
currVer = vertices.getItem(visitedVertex.getNextNeighbor());
currVer->disconnect(visitedVertex.getLabel())
}
}
}
*/


// Algorithm for applyPrim
/*
1/Create a temp vector<PrimEdge>, create a current vertex, create vector<LabelType> visitedVertexVect, bool found = false
2/Choose the first Edge in orderedEdge, choose the first vertex (we will use it to start our MST) and set it as Current vertex
3/Start for loop
Use another loop to check in the orderedEdge, find the list of Prim Edge which has the Start vertex or End vertex same with our current vertex - update found = true
(If cannot find anything - found = false, check in visitedVertexVect, set current vertex to a different one)
4/Push them to tempVector, pop out of orderedEdge , end inner loop, found reset to false
5/Check in the tempVector which PrimEdge has lowest weight. (Don't reset the tempVector)
6/Push that PrimEdge to minSpanTree vector -> pop that edge out of tempVector
7/Push current vertex into visitedVertexVect
8/Current vertex = end vertex of that PrimEdge
9/End for loop
10/Delete temp vector.

The downside of my code is that It must check every single edge in orderedEdge vector for every vertex --> O(n).
I intend to improve it by approaching in a different way - using another function disconnectvisitedvertex.
However, it's a little bit confused and not really as clear as this approach.
*/

// Beginning of function definitions required for applyPrim()

/** Calls applyPrim to manipulate the minSpanTree private member var
*/
template <class LabelType>
void Prim<LabelType>::createMinSpanTree()
{
	applyPrim();
}

template <class LabelType>
void Prim<LabelType>::applyPrim()
{
	//Start prepping the graph for traversal.
	if (minSpanTree.size() > 0)		//Should be starting with a fresh minimum spanning tree; if not, reset it.
	{
		minSpanTree.clear();
	}
	this->unvisitVertices();			// Reset this graph. Uses this-> since it is a fcn of base class LinkedGraph.h
	vector<PrimEdge<LabelType>> tempVect;
	vector<LabelType> visitedVertexVect;	// Change to label type, change all uses to not be a vertex
	bool found = false;
	int numEdges = orderedEdges.size();
	int edgeCount = 0;

	LabelType firstVertex = orderedEdges[0].getStart();		//Grab the starting vertex of the first orderedEdge
	LabelType currVertex = firstVertex;		// Change to label type, change all uses to not be a vertex
	//visitedVertexVect.push_back(firstVertex); // Don't need
	//LinkedStack<LabelType> vertexStack;		// We don't need the vertexStack to go back

	for (int i = 0; i < numEdges && edgeCount < numberOfVertices - 1; ++i, edgeCount++)
	{
		//vertexStack.push(currVertex); // We don't need the vertexStack to go back
		found = getLocalUnvisitedNeighbors(currVertex, tempVect);	//Returns the localOrderedEdges connected to the current vertex
		visitedVertexVect.push_back(currVertex); // must add the vertex into visitedVertexVect immediately before checking
		if (found)	//If found is true, this means we were able to find adjacent vertices to the currVertex 
		{
			findRightVertexAndVectEdge(tempVect, currVertex, visitedVertexVect); // new function, please read
			minSpanTree.push_back(tempVect[0]);
			tempVect.erase(tempVect.begin());
		}
		else //We're at a leaf vertex
		{
			//vertexStack.pop();
			//currVertex = vertexStack.peek();
			//getLocalUnvisitedNeighbors(currVertex, tempVect);
			/*Explain algorithm:
			When getLocalUnvisitedNeighbors cannot find any edges for current vertex, we continue to choose the lowest weight edges in
			tempVect. Then, we trace in visitedVertexVect list which end of the Edge contains the unvisited Vertex. As we know edge has
			start vertex and end vertex, tempVect[0] guarantees that one end will be visited and one will be unvisited. We just need to 
			loop through the visitedVertexVect, find which end of the edges hasn't visited yet then assign it to current vertex.

			Explain why vertexStack not working properly:
			For example: Use the graph on Catalist as example but let imagine a - b - 3 (change from a - b - 6) and we are at node h
			tempVect has:
			a - b - 3
			d - c - 4
			e - g - 8

			Current Vertex = h
			VisitedVertexVect = a, i, f, g, d, h
			Stack: a, f, g, d, h

			h has no edge --> Pop h out --> assign d to current vertex
			Current Vertex = d
			Then what the code does now. It will choose a - b - 3 because it is the lowest weight edge.

			Then how can we choose the current vertex?
			In our previous code we assign current vertex by look at the edges and if the Start of edge is not equal to current vertex,
			set End of edge to current vertex.
			d != a --> currVertex = a;
			currentVertex = a
			tempVect has:
			d - c - 4
			e - g - 8
			If we put a to getLocalUnvisitedNeighbors, it definitely return false and our new edge will be (d - c - 4) which is not
			always right. We have added a new vertex b but we don't add the edges of that vertex into the tempVect.
			*/
			findRightVertexAndVectEdge(tempVect, currVertex, visitedVertexVect);
			minSpanTree.push_back(tempVect[0]);
			tempVect.erase(tempVect.begin());

		}
		found = false;
	}

	//Clear memory
	tempVect.clear();
}

/*
findRightVertexndEdge will check the TempVect and delete all of the unavailable edge(Start and End vertices have visited)
and assign the new vertex into current vertex.
For example:
We have a graph of 4 vertices: a, b, c, d. it link all together. It means we have 6 edges: a-b-1, a-c-3, a-d-4, b-c-2 , b-d-6, c-d-5
Imagine we start at a, we will choose a-b-1 then b-c-2. After this point, we cannot choose a-c-3 because a and c have visited.
However, in tempVect, it still has a-c-3. Therefore, this function will delete it.
After we choose the right Edges from tempVect, we can check which end of that edge is suitable to assign to current vertex.
*/
template <class LabelType>
void Prim<LabelType>::findRightVertexAndVectEdge(vector<PrimEdge<LabelType>> &tempVect, LabelType &currVertex, vector<LabelType> &visitedVertexVect)
{
	bool startFound = false;
	bool endFound = false;
	while ((startFound == false && endFound == false) || (startFound == true && endFound == true))
	{
		endFound = false;
		startFound = false;
		for (unsigned int j = 0; j < visitedVertexVect.size(); j++)
		{
			if (tempVect.at(0).getEnd() == visitedVertexVect.at(j))
				endFound = true;
			if (tempVect.at(0).getStart() == visitedVertexVect.at(j))
				startFound = true;
		}
		if (endFound == true && startFound == true)
		{
			tempVect.erase(tempVect.begin());
		}
	}

	if (endFound == false)
		currVertex = tempVect[0].getEnd();
	if (startFound == false)
		currVertex = tempVect[0].getStart();
}

/** Looks at the current Minimal Spanning Tree, and retrieves a listing of all of the
unvisited neighbor vertices and sorts them in order to find the least - cost edge (v,u)
where v is a visited vertex and u is an unvisited vertex
@param currVertex - A vertex that has currently been visited at that point in time within applyPrim()
@param found - A boolean passed by reference that indicates whether was able to find existing neighboring edges connected to curreVertex
@return localOrderedEdges - defined in private member variables as a vector<PrimEdge<LabelType>>
- Luke
*/
template <class LabelType>
bool Prim<LabelType>::getLocalUnvisitedNeighbors(LabelType currVertex, vector<PrimEdge<LabelType>> &tempVect)
{
	bool status = false;
	//Will be set to whatever tempVect is
	//Find currVertex's neighbors
	for (unsigned int i = 0; i < orderedEdges.size(); i++)
	{
		if ((currVertex == orderedEdges.at(i).getStart() || currVertex == orderedEdges.at(i).getEnd())
			&& orderedEdges.at(i).isChecked() == false)
		{
			tempVect.push_back(orderedEdges.at(i));
			orderedEdges.at(i).setChecked(true);
			status = true;
		}
	}
	selectionSort(tempVect);
	return status;

	//For loop to find the edge that we want and push onto tempVect
	//In for loop: Update edge to having been checked in orderedEdges

	//Find the list of edges adjacent to the currVertex

	//Call selectionSort(localVect)
	//Return true if was able push onto tempVect
}

/** Uses the selection sort algortihm to sort localOrderedEdges in ascending order, defined above.
@param array - The unsorted localOrdredEdges, will pass by reference as we sort
- Luke : "I'm adding these fcns to try out, as they seem to be needed in applyPrim()"
Algorithm pulled off of "http://cncpp.divilabs.com/2013/12/c-program-code-for-selection-sort.html"
*/
template <class LabelType>
void Prim<LabelType>::selectionSort(vector<PrimEdge<LabelType>> &array)
{
	PrimEdge<LabelType> temp;
	int size = array.size();

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (array[j].getWeight() < array[i].getWeight())
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}

	return;
}

/**  Checks if end1 vertex is already connected to end2 vertex in the minimum spanning tree so far
@param end1 - A vertex in the minimum spanning tree
@param end2 - A different vertex in the minimum spanning tree
@return True if the ends are not connected, false if the ends are connected
- Luke : "Not sure if this is needed in applyPrim() yet, just leaving it here for now"
*/
template <class LabelType>
bool Prim<LabelType>::notConnected(LabelType &end1, LabelType &end2)
{
	for (int i = 0; i < minSpanTree.size(); ++i)
	{
		// THIS IS REALLY DIFFICULT, BUT NOT SHOWN BECAUSE YOU MAY HAVE
		//    SIMILAR ALGORITHMS THAT NEED THIS
		// ETC.
	}
	return true;
}

//End of function definitions required to solve applyPrim()

//Beginning of function definitions to print out things
template <class LabelType>
void Prim<LabelType>::writeMinSpanTree(ostream &os)
{
	writeVector(os, minSpanTree);
}

template <class LabelType>
void Prim<LabelType>::writeLocalOrderedEdges(ostream &os)
{
	writeVector(os, localOrderedEdges);
}

template <class LabelType>
void Prim<LabelType>::writeVector(ostream &os, vector<PrimEdge<LabelType>> &vect)
{
	int size = vect.size();
	for (int i = 0; i < size; ++i){
		PrimEdge<LabelType> edge = vect[i];
		os << "From " << edge.getStart() << " to " << edge.getEnd()
			<< " with weight = " << edge.getWeight() << endl;
	}
}
// End of function definitions to print out things

template<class LabelType>
bool Prim<LabelType>::remove(LabelType start, LabelType end)
{
	vector<PrimEdge<LabelType>>::iterator iterElem;
	for (iterElem = orderedEdges.begin(); iterElem != orderedEdges.end(); ++iterElem)
	{
		LabelType end1 = iterElem->getStart();
		LabelType end2 = iterElem->getEnd();
		if (start == end1 && end == end2 || start == end2 && end == end1){
			undoStack.push(*iterElem);
			orderedEdges.erase(iterElem);
			break;
		}
	}
	return LinkedGraph<LabelType>::remove(start, end);
}


template<class LabelType>
bool Prim<LabelType>::add(LabelType start, LabelType end, float edgeWeight = 0)
{
	if (LinkedGraph<LabelType>::add(start, end, edgeWeight))
	{
		Edge<LabelType> edge(end, edgeWeight);
		PrimEdge<LabelType> newEdge(start, edge);
		orderedEdges.push_back(newEdge);
		return true;
	}
	return false;
}

template <class LabelType>
bool Prim<LabelType>::undo()
{
	if (undoStack.size() > 0)
	{
		PrimEdge<LabelType> temp = undoStack.peek();
		LabelType start = temp.getStart();
		LabelType end = temp.getEnd();
		float weight = temp.getWeight();
		cout << "\nUndo\n";

		if (add(start, end, weight))
		{
			undoStack.pop();
			return true;
		}
		else
			return false;
	}
	else
	{
		cout << "There is nothing to undo" << endl;
		return false;
	}
}