#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "LinkedGraph.h"
#include "BinarySearchTree.h"
#include "Vertex.h"
#include "Edge.h"
#include "LinkedStack.h"

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
		int getWeight() const { return edge.getWeight(); }
		bool isChecked() const { return checked; }

		bool operator<(const PrimEdge &right)
		{
			return this->edge.getWeight() < right.getWeight();
		}

	};

private:
	LinkedStack<PrimEdge<LabelType>> undoStack;
	vector<PrimEdge<LabelType>> minSpanTree;
	vector<PrimEdge<LabelType>> orderedEdges;
	void applyPrim();
//	bool disconnectVisitedVertex(Vertex<LabelType>& visitedVertex);

public:
	Prim();
	~Prim();
	void createMinSpanTree();
	void writeMinSpanTree(ostream &os);
	void writeOrderedEdges(ostream &os); // for debugging
	bool undo();
	bool add(LabelType start, LabelType end, int edgeWeight = 0)
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

	bool remove(LabelType start, LabelType end)
	{
		vector<PrimEdge<LabelType>>::iterator iterElem;
		for (iterElem = orderedEdges.begin(); iterElem != orderedEdges.end(); ++iterElem)
		{
			LabelType end1 = iterElem->getStart();
			LabelType end2 = iterElem->getEnd();
			if (start == end1 && end == end2 || start == end2 && end == end1){
				orderedEdges.erase(iterElem);
				break;
			}
		}
		return LinkedGraph<LabelType>::remove(start, end);
	}

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
	1/Create a temp vector<PrimEdge>, create a current vertex
	2/Choose the first Edge in orderedEdge, choose the first vertex (we will use it to start our MST) and set it as Current vertex
	3/Start for loop, Check in the orderedEdge, find the list of Prim Edge which has the Start vertex same with our current vertex
	(Just use loop to check orderedEdge. Because it is in ordered, after the next edge isn't same, stop the loop)
	4/Push them to tempVector
	5/Check in the tempVector which PrimEdge has lowest weight. (Don't reset the tempVector)
	6/Push that PrimEdge to minSpanTree vector -> pop that edge out of tempVector
	7/Set current vertex = end vertex of that PrimEdge
	8/End for loop
	9/Delete temp vector. 
*/