#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "LinkedGraph.h"
#include "Vertex.h"
#include "Edge.h"
#include "LinkedStack.h"
#include <queue>

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
	struct stackData
	{
		string start, end;
		float weight;
		bool isRemove;
	};
	//Private member variables
	LinkedStack<stackData> undoStack;

	vector<PrimEdge<LabelType>> minSpanTree;	
	vector<PrimEdge<LabelType>> edgesVect;	

	//Private member functions
	void applyPrim();
	bool getLocalUnvisitedNeighbors(LabelType currVertex, vector<PrimEdge<LabelType>> &tempVect);
	void selectionSort(vector<PrimEdge<LabelType>> &array);
	bool notConnected(LabelType &end1, LabelType &end2);
	void writeVector(ostream &os, vector<PrimEdge<LabelType>> &vect);

	void findRightVertexAndVectEdge(vector<PrimEdge<LabelType>>& tempVect, LabelType &currVertex, vector<LabelType> &visitedVertexVect);


public:
//	Prim(){}
//	~Prim(){}
	void clear();
	void createMinSpanTree();
	void writeMinSpanTree(ostream &os);
	void writeEdgesVect(ostream &os); // for debugging
	bool undo();
	bool add(LabelType start, LabelType end, float edgeWeight = 0);
	bool remove(LabelType start, LabelType end);
	
	void removeEdges();
	void addUsingExisting();
	void addUsingNew();
	void addUsingExistingAndNew();
	
};

template <class LabelType>
void Prim<LabelType>::clear()
{
	LinkedGraph::clear();
	minSpanTree.clear();
	edgesVect.clear();
	undoStack.clear();

}


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
	//outputMinSpanTree();
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
	int numEdges = edgesVect.size();
	int edgeCount = 0;

	LabelType firstVertex = edgesVect[0].getStart();		//Grab the starting vertex of the first orderedEdge
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
*/
template <class LabelType>
bool Prim<LabelType>::getLocalUnvisitedNeighbors(LabelType currVertex, vector<PrimEdge<LabelType>> &tempVect)
{
	bool status = false;
	//Will be set to whatever tempVect is
	//Find currVertex's neighbors
	for (unsigned int i = 0; i < edgesVect.size(); i++)
	{
		if ((currVertex == edgesVect.at(i).getStart() || currVertex == edgesVect.at(i).getEnd())
			&& edgesVect.at(i).isChecked() == false)
		{
			tempVect.push_back(edgesVect.at(i));
			edgesVect.at(i).setChecked(true);
			status = true;
		}
	}
	selectionSort(tempVect);
	return status;
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
void Prim<LabelType>::writeVector(ostream &os, vector<PrimEdge<LabelType>> &vect)
{
	// Use iomanip to make look nice
	int size = vect.size();
	cout << "\t Vertex 1 \t \t Vertex 2 \t\t Weight" << endl;
	for (int i = 0; i < size; ++i){
		PrimEdge<LabelType> edge = vect[i];
		os << "Edge " << i+1 << ": " << edge.getStart() << " -- " << edge.getEnd() << " ( " << edge.getWeight() << " ) " << endl;
	}
}
// End of function definitions to print out things

template<class LabelType>
bool Prim<LabelType>::remove(LabelType start, LabelType end)
{
	bool bTest = false;
	vector<PrimEdge<LabelType>>::iterator iterElem;
	for (iterElem = edgesVect.begin(); iterElem != edgesVect.end(); ++iterElem)
	{
		LabelType end1 = iterElem->getStart();
		LabelType end2 = iterElem->getEnd();
		if (start == end1 && end == end2 || start == end2 && end == end1){

			edgesVect.erase(iterElem);
			bTest = LinkedGraph<LabelType>::remove(start, end);
			break;
		}
	}
	return bTest;
//	return LinkedGraph<LabelType>::remove(start, end);
}


template<class LabelType>
bool Prim<LabelType>::add(LabelType start, LabelType end, float edgeWeight = 0)
{
	if (LinkedGraph<LabelType>::add(start, end, edgeWeight))
	{
		Edge<LabelType> edge(end, edgeWeight);
		PrimEdge<LabelType> newEdge(start, edge);
		edgesVect.push_back(newEdge);
		return true;
	}
	return false;
}

/**  Checks the undo stack to see if the last edge in the stack was recently removed or added.
If it was removed, we add it back in. If it was added, then we remove it.
@return True if we were able to undo the last operation, false if we were not able to undo.
*/
template <class LabelType>
bool Prim<LabelType>::undo()
{
	if (undoStack.size() > 0)
	{
		stackData tempEdge = undoStack.peek();
		LabelType start = tempEdge.start;
		LabelType end = tempEdge.end;
		float weight = tempEdge.weight;

		if (tempEdge.isRemove)
		{
			if (add(start, end, weight))
			{
				cout << "Re-added edge: " << tempEdge.start << " - " << tempEdge.end << " (" << tempEdge.weight << " )" << endl;
				undoStack.pop();
				return true;
			}
			else
			{
				cout << "ERROR: Failed to re-add edge: " << tempEdge.start << " - " << tempEdge.end << " (" << tempEdge.weight << " )" << endl;
				return false;
			}
				
		}
		else
		{
			if (remove(start, end))
			{
				cout << "Removed edge: " << tempEdge.start << " - " << tempEdge.end << " (" << tempEdge.weight << " )" << endl;
				undoStack.pop();
				return true;
			}
			else
			{
				cout << "ERROR: Failed to remove edge: " << tempEdge.start << " - " << tempEdge.end << " (" << tempEdge.weight << " )" << endl;
				return false;
			}
				
		}

	}
	else
	{
		cout << "ERROR: There is nothing to undo" << endl;
		return false;
	}
}


/* Helper function to write out the currect vector of edges*/
template <class LabelType>
void Prim<LabelType>::writeEdgesVect(ostream &os)
{
	writeVector(os, edgesVect);
}


template <class LabelType>
void Prim<LabelType>::removeEdges()
{
	cout << "\tChoose an edge to remove" << endl;
	if (this->getNumEdges() < 1)
	{
		cout << "ERROR: Not enough edges to remove" << endl;
	}
	else
	{
		for (unsigned int i = 0; i < edgesVect.size(); i++)
		{
			PrimEdge<LabelType> edge = edgesVect[i];
			cout << i + 1 << ") - " << ": " << edge.getStart() << " -- " << edge.getEnd() << " (" << edge.getWeight() << ") " << endl;
		}
		int indexChoice = choice(1, edgesVect.size());
		PrimEdge<LabelType> edge = edgesVect[indexChoice - 1];
		stackData edgeToRemove;
		edgeToRemove.start = edge.getStart();
		edgeToRemove.end = edge.getEnd();
		edgeToRemove.weight = edge.getWeight();
		edgeToRemove.isRemove = true;

		if (remove(edgeToRemove.start, edgeToRemove.end))
		{
			cout << "Removed edge: " << edgeToRemove.start << " - " << edgeToRemove.end << " (" << edgeToRemove.weight << " )" << endl;
			undoStack.push(edgeToRemove);
		}
		else
		{
			cout << "Failed to remove edge: " << edgeToRemove.start << " - " << edgeToRemove.end << " (" << edgeToRemove.weight << " )" << endl;
		}
	}
}

template <class LabelType>
void Prim<LabelType>::addUsingExisting()
{
	string strHeader[] = { 
		"================================================================================", 
		"\tAdd an edge using existing vertices\n", 
		"================================================================================" 
	};

	for (string temp : strHeader)
		cout << temp;

	if (this->getNumVertices() < 2)
	{
		cout << "ERROR: Not enough vertices to add an edge" << endl;
	}
	else
	{
		stackData inputAddEdge;
		vector<string> vectVertices;
		int indexChoice;
		float fInput;

		// Display first list of all vertices
		this->traverseAll(visitAddToVector, vectVertices);
		cout << "Choose first vertex for the edge" << endl;
		for (unsigned int i = 0; i < vectVertices.size(); i++)
			cout << i + 1 << ") - " << vectVertices[i] << endl;

		indexChoice = choice(1, vectVertices.size());	// Get index for user choice from list
		inputAddEdge.start = vectVertices[indexChoice - 1];
		vectVertices.erase(vectVertices.begin() + indexChoice - 1);
		system("cls");

		// Add check to display only vertexes that AREN'T connected
		// Get vector of adjacency list
		// Remove items in the adjacency list from the main display vector

		for (string temp : strHeader)
			cout << temp;
		// Display second list of all vertices minus user choice
		cout << "Choose second vertex for the edge" << endl;
		for (unsigned int i = 0; i < vectVertices.size(); i++)
			cout << i + 1 << ") - " << vectVertices[i] << endl;

		indexChoice = choice(1, vectVertices.size());	// Get index for user choice from list
		inputAddEdge.end = vectVertices[indexChoice - 1];
		system("cls");

		for (string temp : strHeader)
			cout << temp;
		cout << "Choose an edge weight: ";
		cin >> fInput;
		cin.ignore(100000, '\n');
		cin.clear();
		while (fInput <= 0)
		{
			cout << "Edge weight must be > 0" << endl;
			cout << "Choose an edge weight: ";
			cin >> fInput;
			cin.ignore(100000, '\n');
			cin.clear();
		}
		inputAddEdge.weight = fInput;
		if (this->add(inputAddEdge.start, inputAddEdge.end, inputAddEdge.weight))
		{
			cout << "Successfully added edge: " << inputAddEdge.start << " - " << inputAddEdge.end << " (" << inputAddEdge.weight << ")" << endl;
			inputAddEdge.isRemove = false;
			this->undoStack.push(inputAddEdge);
		}
		else
			cout << "ERROR: Failed to add edge" << endl;
	}
}

template <class LabelType>
void Prim<LabelType>::addUsingNew()
{
	string strHeader[] = { 
		"================================================================================", 
		"\tAdd an edge using new vertices\n", 
		"================================================================================" 
	};
	stackData inputAddEdge;

	for (string temp : strHeader)
		cout << temp;
	if (this->getNumVertices() > 0)
	{
		cout << "ERROR: Can only add to an empty graph" << endl;
		return;
	}
	cout << "Enter vertex 1: " << endl;
	cin >> inputAddEdge.start;
	cin.ignore(100000, '\n');
	cin.clear();
	// PERFORM INPUT VALIDATION HERE
	// Add check to make sure vertex doesn't exist

	cout << "Enter vertex 2: " << endl;
	cin >> inputAddEdge.end;
	cin.ignore(100000, '\n');
	cin.clear();
	// PERFORM INPUT VALIDATION HERE
	// Add check to make sure verttex doesn't exist

	cout << "Enter edge weight: " << endl;
	cin >> inputAddEdge.weight;
	cin.ignore(100000, '\n');
	cin.clear();
	// PERFORM INPUT VALIDATION HERE

	if (this->add(inputAddEdge.start, inputAddEdge.end, inputAddEdge.weight))
	{
		cout << "Successfully added edge" << endl;
		inputAddEdge.isRemove = false;
		undoStack.push(inputAddEdge);
	}
	else
		cout << "ERROR: Failed to add edge" << endl;
}

template <class LabelType>
void Prim<LabelType>::addUsingExistingAndNew()
{
	string strHeader[] = { 
		"================================================================================",
		"\tAdd an edge using an existing vertex and a new vertex\n", 
		"================================================================================" 
	};

	for (string temp : strHeader)
		cout << temp;

	if (this->getNumVertices() < 1)
	{
		cout << "ERROR: Not enough vertices to add an edge" << endl;
	}
	else
	{
		stackData inputAddEdge;
		vector<string> vectVertices;
		int indexChoice;

		this->traverseAll(visitAddToVector, vectVertices);
		cout << "Choose first vertex for the edge" << endl;
		for (unsigned int i = 0; i < vectVertices.size(); i++)
			cout << i + 1 << ") - " << vectVertices[i] << endl;

		indexChoice = choice(1, vectVertices.size());	// Get index for user choice from list
		inputAddEdge.start = vectVertices[indexChoice - 1];
		vectVertices.erase(vectVertices.begin() + indexChoice - 1);
		system("cls");

		for (string temp : strHeader)
			cout << temp;
		cout << "Vertex 1: " << inputAddEdge.start << endl;
		cout << "Enter vertex 2: " << endl;
		cin >> inputAddEdge.end;
		cin.ignore(100000, '\n');
		cin.clear();
		// PERFORM INPUT VALIDATION HERE

		cout << "Enter edge weight: " << endl;
		cin >> inputAddEdge.weight;
		cin.ignore(100000, '\n');
		cin.clear();
		// PERFORM INPUT VALIDATION HERE

		if (this->add(inputAddEdge.start, inputAddEdge.end, inputAddEdge.weight))
		{
			cout << "Successfully added edge" << endl;
			inputAddEdge.isRemove = false;
			undoStack.push(inputAddEdge);
		}
		else
			cout << "ERROR: Failed to add edge" << endl;
	}
}