#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "LinkedGraph.h"
//#include "BinarySearchTree.h"
#include "Vertex.h"
#include "Edge.h"
#include "LinkedStack.h"

// By C. Lee-Klawender

// For showing an example of how you could use the LinkedGraph class for your
//     graph problem: YOU ARE NOT ALLOWED TO USE THIS CLASS!

// THIS IS PART OF THE CLASS FOR SOLVING the Minimum Spanning Tree problem
//     using Prim's algorithm (not all of the functions are given here)

template <class LabelType>
class Prim : public LinkedGraph<LabelType>
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
	vector<PrimEdge<LabelType>> minSpanTree;
	vector<PrimEdge<LabelType>> orderedEdges;
	LinkedStack<PrimEdge<LabelType>> undoStack;

	void applyPrim();
	bool notConnected(LabelType &end1, LabelType &end2);
	void selectionSort(vector<PrimEdge<LabelType>> &array, int size);
	void writeVector(ostream &os, vector<PrimEdge<LabelType>> &vect);

public:
	Prim(){}
	~Prim(){}
	void createMinSpanTree();
	void writeMinSpanTree(ostream &os);
	void writeOrderedEdges(ostream &os); // for debugging
	bool add(LabelType start, LabelType end, float edgeWeight = 0);
	bool remove(LabelType start, LabelType end);
	bool undo();
};

template <class LabelType>
void Prim<LabelType>::createMinSpanTree()
{
	selectionSort(orderedEdges, orderedEdges.size());
	applyPrim();
}

template <class LabelType>
void Prim<LabelType>::selectionSort(vector<PrimEdge<LabelType>> &array, int size)
{
	int startScan, minIndex;
	PrimEdge<LabelType> minValue;

	// REST OF THE SELECTION SORT GOES HERE

}


template <class LabelType>
void Prim<LabelType>::writeMinSpanTree(ostream &os)
{
	writeVector(os, minSpanTree);
}

template <class LabelType>
void Prim<LabelType>::writeOrderedEdges(ostream &os)
{
	writeVector(os, orderedEdges);
}
// private functions:

template <class LabelType>
void Prim<LabelType>::applyPrim()
{
	if (minSpanTree.size() > 0)
	{
		minSpanTree.clear();
	}
	unvisitVertices(); // reset this graph
	int numEdges = orderedEdges.size();
	int edgeCount = 0;
	for (int i = 0; i < numEdges && edgeCount < numberOfVertices - 1; ++i)
	{
		for (int i = 0; i < minSpanTree.size(); ++i){ // reset for each edge
			minSpanTree[i].setChecked(false);
		}

		PrimEdge<LabelType> *currEdge = &orderedEdges[i];
		LabelType start = currEdge->getStart(); // get name of start of edge
		LabelType end = currEdge->getEnd(); // get name of end of edge
		Vertex<LabelType>* startVertex = 0, *endVertex = 0;

		startVertex = vertices.getItem(start); // get start Vertex from graph
		endVertex = vertices.getItem(end); // get end Vertex from graph
		if (/* REMOVED */)
		{
			// NOT SHOWN

		}

	}
}

// checks if end1 is already connected to end2 in the minimum spanning tree so far
template <class LabelType>
bool  Prim<LabelType>::notConnected(LabelType &end1, LabelType &end2)
{
	for (int i = 0; i < minSpanTree.size(); ++i)
	{
		// THIS IS REALLY DIFFICULT, BUT NOT SHOWN BECAUSE YOU MAY HAVE
		//    SIMILAR ALGORITHMS THAT NEED THIS
		// ETC.
	}
	return true;
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


template <class LabelType>
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
	cout << "\nRemove\n";
	return LinkedGraph<LabelType>::remove(start, end);
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