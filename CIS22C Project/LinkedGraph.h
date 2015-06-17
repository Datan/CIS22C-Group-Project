//  Created by Frank M. Carrano and Tim Henry.
//  Copyright (c) 2013 __Pearson Education__. All rights reserved.
//  Modified by C. Lee-Klawender
//  UPDATED 06/08/2015 (in remove, added delete statements)
//  UPDATED 06/11/2015 (in add, didn't allow adding an edge that connects to itself AND)
//                       AND in remove, decremented numberOfVertices if not connected)
/** @file LinkedGraph.h */

#ifndef _LINKED_GRAPH
#define _LINKED_GRAPH

#include "GraphInterface.h"
#include "Vertex.h"
#include "DACmap.h"// For adjacency list in this class and also in the Vertex
							// You're required to use this implementation
							 // of an adjacency list.
#include <queue>

using namespace std;

template<class LabelType>
class LinkedGraph : public GraphInterface<LabelType>
{
protected: // protected so you can derive this class for you team project solution
   int numberOfVertices;
   int numberOfEdges;

   // For the CIS 22C Team Project, you're REQUIRED to use this implementation
   //     of the vertices and iterator
   DACmap<LabelType, Vertex<LabelType>* > vertices;
   DACmapIterator<LabelType, Vertex<LabelType>* > *pvertexIterator;

   // Marks all verices as unvisited.
   void unvisitVertices();

   Vertex<LabelType>* findOrCreateVertex(const LabelType& vertexLabel);

   void depthFirstTraversalHelper(Vertex<LabelType>* startVertex,  void visit(LabelType&));
   void breadthFirstTraversalHelper(Vertex<LabelType>* startVertex, void visit(LabelType&));
public:
   LinkedGraph();
   virtual ~LinkedGraph();
   int getNumVertices() const;
   int getNumEdges() const;
   virtual bool add(LabelType start, LabelType end, float edgeWeight = 0);

   // For remove to return true all of the following must be true:
   // 1) start and end vertices exist
   // 2) Edge start->end is successfully disconnected
   // 3) Edge end->start is successfully disconnected
   // Then, if those are successful and either start or end no longer
   // has neighbors, the vertex is removed from the graph
   virtual bool remove(LabelType start, LabelType end);

   float getEdgeWeight(LabelType start, LabelType end) const;
   void depthFirstTraversal(LabelType start, void visit(LabelType&));
   void breadthFirstTraversal(LabelType start, void visit(LabelType&));

   bool isInGraph(LabelType & item);
   //---> YOU DECLARE HERE (AND WRITE BELOW) THE MEMBER FUNCTION TO
   //         WRITE THE GRAPH TO A TEXT FILE (SUGGEST TO PASS AN
   //        ofstream TO THIS !


}; // end GraphInterface

template<class LabelType>
LinkedGraph<LabelType>::
LinkedGraph(): numberOfVertices(0), numberOfEdges(0)
{
	pvertexIterator = 0;
}  // end default constructor

template<class LabelType>
LinkedGraph<LabelType>::~LinkedGraph(){

}

template<class LabelType>
int LinkedGraph<LabelType>::getNumVertices() const
{
   return numberOfVertices;
}  // end getNumVertices

template<class LabelType>
int LinkedGraph<LabelType>::getNumEdges() const
{
   return numberOfEdges;
}  // end getNumEdges

template<class LabelType>
bool LinkedGraph<LabelType>::add(LabelType start, LabelType end, float edgeWeight)
{
   if( start == end )               // UPDATED 06/11/2015
		return false;

   Vertex<LabelType>* startVertex = findOrCreateVertex(start);
   Vertex<LabelType>* endVertex   = findOrCreateVertex(end);

   if( startVertex->connect(end, edgeWeight) && endVertex->connect(start, edgeWeight) )
   {
		numberOfEdges++; // Each bidirectional edge counts as a single edge

		return true;
   }
   return false;
}  // end add

template<class LabelType>
bool LinkedGraph<LabelType>::remove(LabelType start, LabelType end)
{
   bool successful = false;
   Vertex<LabelType>* startVertex = vertices.getItem(start);
   Vertex<LabelType>* endVertex   = vertices.getItem(end);

   successful = startVertex->disconnect(end);
   if (successful)
   {
	  successful = endVertex->disconnect(start);
	  if (successful)
	  {
		 numberOfEdges--;

		 // If either vertex no longer has a neighbor, remove it
		 if (startVertex->getNumberOfNeighbors() == 0)
		 {
			 vertices.remove(start);
			 --numberOfVertices; // UPDATED 06/11/2015
			 delete startVertex; // UPDATED 06/08/2015
		 }

		 if (endVertex->getNumberOfNeighbors() == 0)
		 {
			vertices.remove(end);
			--numberOfVertices; // UPDATED 06/11/2015
			delete endVertex;  // UPDATED 06/08/2015
		 }
	  }
	  else
		 successful = false; // Failed disconnect from endVertex
   }
   else
	  successful = false;    // Failed disconnect from startVertex

   return successful;
}  // end remove

template<class LabelType>
float LinkedGraph<LabelType>::getEdgeWeight(LabelType start, LabelType end) const
{
	float weight = -1;
   if (vertices.contains(start))
   {
	  Vertex<LabelType>* startVertex = vertices.getItem(start);
	  weight = startVertex->getEdgeWeight(end);
   }  // end if

   return weight;
}  // end getEdgeWeight

// Mark all vertices as unvisited
template<class LabelType>
void LinkedGraph<LabelType>::unvisitVertices()
{
	pvertexIterator = vertices.iterator();
	while (pvertexIterator->hasNext())
	{
	  Vertex<LabelType>* loopVertex = pvertexIterator->next();
	  loopVertex->unvisit();
	}  // end while
} // endunvisitVertices

template<class LabelType>
void LinkedGraph<LabelType>::depthFirstTraversal(LabelType startLabel, void visit(LabelType&))
{
   // Mark all vertices as unvisited
   unvisitVertices();

   Vertex<LabelType>* startVertex = vertices.getItem(startLabel);
   depthFirstTraversalHelper(startVertex, visit);
}  // end depthFirstTraversal

template<class LabelType>
void LinkedGraph<LabelType>::breadthFirstTraversal(LabelType startLabel, void visit(LabelType&))
{
   // Mark all vertices as unvisited
   unvisitVertices();

   Vertex<LabelType>* startVertex = vertices.getItem(startLabel);
   breadthFirstTraversalHelper(startVertex, visit);
}  // end breadthFirstTraversal

template<class LabelType>
void LinkedGraph<LabelType>::
depthFirstTraversalHelper(Vertex<LabelType>* startVertex, void visit(LabelType&))
{
   startVertex->visit();  // Mark as visited
   LabelType startLabel = startVertex->getLabel();
   visit(startLabel);

   startVertex->resetNeighbor(); // Reset reference for adjacency list

   for (int index = 1; index <= startVertex->getNumberOfNeighbors(); index++)
   {
	  LabelType currentLabel = startVertex->getNextNeighbor();
	  if (!((vertices.getItem(currentLabel))->isVisited()))
		 depthFirstTraversalHelper(vertices.getItem(currentLabel), visit);
   }  // end for
}  // end depthFirstTraversalHelper

template<class LabelType>
void LinkedGraph<LabelType>::
breadthFirstTraversalHelper(Vertex<LabelType>* startVertex, void visit(LabelType&))
{
   queue<Vertex<LabelType>*> vertexQueue;
   LabelType startLabel = startVertex->getLabel();
//   cout << "Enqueue and visit " << startLabel << endl;
   vertexQueue.push(startVertex);
   startVertex->visit();         // Mark as visited
   visit(startLabel);
   startVertex->resetNeighbor(); // Reset reference for adjacency list

   while (!vertexQueue.empty())
   {
	  // Remove vertex from queue
	  Vertex<LabelType>* nextVertex = vertexQueue.front();
	  vertexQueue.pop();
	  LabelType nextLabel = nextVertex->getLabel();
//      cout << "Dequeue " << nextLabel << endl;
//      cout << "Consider " << nextLabel << "'s " << nextVertex->getNumberOfNeighbors() << " neighbors." << endl;

	  // Add neighbors of visited vertex to queue
	  for (int index = 1; index <= nextVertex->getNumberOfNeighbors(); index++)
	  {
		 LabelType neighborLabel = nextVertex->getNextNeighbor();
//         cout << "Neighbor " << neighborLabel;
		 Vertex<LabelType>* neighbor = vertices.getItem(neighborLabel);
		 if (!neighbor->isVisited())
		 {
//            cout << " is not visited; enqueue and visit it." << endl;
			vertexQueue.push(neighbor);
			neighbor->visit();         // Mark as visited
			visit(neighborLabel);
			neighbor->resetNeighbor(); // Reset reference for adjacency list
		 }
//         else
//            cout << " was visited already; ignore it." << endl;
	  }  // end for
   }  // end while
}  // end breadthFirstTraversalHelper

template<class LabelType>
Vertex<LabelType>* LinkedGraph<LabelType>::
findOrCreateVertex(const LabelType& vertexLabel)
{
   Vertex<LabelType>* theVertex = nullptr;

   if (vertices.contains(vertexLabel))
   {
	  theVertex = vertices.getItem(vertexLabel);
   }
   else
   {
	  theVertex = new Vertex<LabelType>(vertexLabel);
	  vertices.add(vertexLabel, theVertex);
	  numberOfVertices++;
   }  // end if

   return theVertex;
}  // end findOrCreateVertex

// Tests if an item is a vertex
template<class LabelType>
bool LinkedGraph<LabelType>::isInGraph(LabelType & item)
{
	return vertices.contains(item) ? true : false;
}

// WRITE THE MEMBER FUNCTION HERE TO
   //         WRITE THE GRAPH TO A TEXT FILE (SUGGEST TO PASS AN
   //        ofstream TO THIS !
#endif
