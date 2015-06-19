//  Created by Frank M. Carrano and Tim Henry.
//  Copyright (c) 2013 __Pearson Education__. All rights reserved.

#include <iostream>
#include <string>
#include "LinkedGraph.h"
#include "LoopingMenu.h"
#include "Prim.h"
//#include "Prim.h"

using namespace std;

typedef function<void()> my_func;

const string strMainMenu = "Main Menu.txt";
const string strModifyMenu = "Modify Menu.txt";
const string strDisplayMenu = "Display Menu.txt";


struct inputEdge
{
	string start;
	string end;
	float weight;
	bool isAdd;
};

void pause(){
	cout << "Press <ENTER> to continue" << endl;
	cin.clear();
	cin.ignore(1, '\n');
}

int choice(int low, int high)
{	/*
	Function: Allows user to input int values between two values
	Parameters:
	low - Lowest int value to be used
	high - Highest int value to be used
	Returns: Valid integer input (integers between 'low' and 'high') (including 'low' and 'high')
	*/
	int cI;
	do			// Loop runs until valid integer input is received (integers between 'low' and 'high')
	{
		cout << "Choice: ";
		cin >> cI;
		cin.clear();
		cin.ignore(10000, '\n');

		if (cI < low || cI > high)
			cout << "Invalid choice, try again" << endl;
	} while (cI < low || cI > high);
	return cI;
}

template<typename ItemType>
void display(ItemType& anItem)
{
   cout << "Vertex - " << anItem << endl;
}

template<typename ItemType>
void writeFile(ItemType& anItem, ofstream &ofs)
{
	ofs << "Vertex - " << anItem << endl;
}

// To be used as the visit function of a traversal function
template<typename ItemType>
void visitAddToVector(ItemType & anItem, vector<ItemType> &vect)	
{
	vect.push_back(anItem);
}
void primTest(Prim<string>* graph)
{
	graph->add("a", "i", 2);
	graph->add("a", "b", 6);
	graph->add("a", "f", 4);
	graph->add("b", "c", 7);
	graph->add("b", "e", 9);
	graph->add("c", "e", 3);
	graph->add("c", "d", 4);
	graph->add("e", "g", 8);
	graph->add("d", "h", 1);
	graph->add("g", "d", 5);
	graph->add("f", "g", 2);

	cout << "Vertices :" << graph->getNumVertices() << endl;
	cout << "Edges :" << graph->getNumEdges() << endl;

	cout << "\nDepth-first traversal" << endl;		//  (should be A B E F J C G K L D H M I N):
	graph->depthFirstTraversal("a", display);

	cout << "\nBreadth-first traversal" << endl;	//  (should be A B C D E F G H I J K L M N):
	graph->breadthFirstTraversal("a", display);

	pause();
	cout << "Creating Minimum Spanning Tree" << endl;
	graph->createMinSpanTree();
	cout << "Displaying Minimum Spanning Tree" << endl;
	graph->writeMinSpanTree(cout);
}
void primFileTest(Prim<string>* graph)
{
	cout << "Vertices :" << graph->getNumVertices() << endl;
	cout << "Edges :" << graph->getNumEdges() << endl;

	cout << "\nDepth-first traversal" << endl;		//  (should be A B E F J C G K L D H M I N):
	graph->depthFirstTraversal("2227.83.27.247", display);

	cout << "\nBreadth-first traversal" << endl;	//  (should be A B C D E F G H I J K L M N):
	graph->breadthFirstTraversal("2227.83.27.247", display);

	pause();
	cout << "Creating Minimum Spanning Tree" << endl;
	graph->createMinSpanTree();
	cout << "Displaying Minimum Spanning Tree" << endl;
	graph->writeMinSpanTree(cout);
	pause();
}
void graphTest(LinkedGraph<string>* testGraph)
{
   string A("A");
   string B("B");
   string C("C");
   string D("D");
   string E("E");
   string F("F");
   string G("G");
   string H("H");
   string I("I");
   string J("J");
   string K("K");
   string L("L");
   string M("M");
   string N("N");
   
   testGraph->add(A, B, 0);
   testGraph->add(A, C, 0);
   testGraph->add(A, D, 0);
   testGraph->add(B, E, 0);
   testGraph->add(B, F, 0);
   testGraph->add(C, G, 0);
   testGraph->add(D, H, 0);
   testGraph->add(D, I, 0);
   testGraph->add(F, J, 0);   
   testGraph->add(G, K, 0);
   testGraph->add(G, L, 0);   
   testGraph->add(H, M, 0);
   testGraph->add(I, M, 0);
   testGraph->add(I, N, 0);
   
   cout << "Vertices :" << testGraph->getNumVertices() << endl;
   cout << "Edges :" << testGraph->getNumEdges() << endl;
  
   cout << "\nDepth-first traversal" << endl;	//  (should be A B E F J C G K L D H M I N):
   testGraph->depthFirstTraversal(A, display);
  
   cout << "\nBreadth-first traversal" << endl;	//  (should be A B C D E F G H I J K L M N):
   testGraph->breadthFirstTraversal(A, display);
}  // end graphTest

	/** Gets user input for a filename and opens that file
		@post A file is open
		@param &ifs Address of the ifstream object being used to open the file
		@return True if the file was successfully opened, false otherwise	*/
bool openInputFile(ifstream &ifs)
{
	string filename;	// String used to store the input for the file name
	cout << "Enter the input filename: ";
	getline(cin, filename);
	ifs.open(filename);
	return ifs.is_open();
}
void readFileIntoGraph(Prim<string>* graph)
{
	ifstream ifs;
	if (openInputFile(ifs))
	{
		graph->readFile(ifs);
		cout << "File reading successful" << endl;
	}
	else
		cout << "ERROR: Failed to read from file" << endl;
	ifs.clear();
	ifs.close();
}
void readFileIntoGraph(Prim<string>* graph, ifstream &inFile)
{
	if (!inFile || !inFile.is_open())
		return;
	if (inFile.eof())
	{
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
	}
	string strTemp;
//	vector<string> vecVertices;	// Will be used to test if all the vertices from the file exist in the graph
	while (strTemp != "BREAK")
	{
		inFile >> strTemp;
//		vecVertices.push_back(strTemp);
	}
	
//	Prim<string>* tempPrim = new Prim < string > ;	Will be used to initially read, then will be copied to graph if conditions are met
	string strStart, strEnd;
	char cTemp;
	float dWeight;
	graph->clear();
	while (inFile >> dWeight >> strStart >> cTemp >> strEnd)
	{
//		iWeight = static_cast<int> (dWeight * 100);
		graph->add(strStart, strEnd, dWeight);
	}

	// Add testing to see that every vertex in the file is in the graph (doesn't test edges) (yet?)
	// if (tempPrim has all vertexes)
	// delete graph;
	// graph = tempPrim;
}

void writeToTextFile(Prim<string>* graph)
{
	ofstream myFile;
	string fileName;
	cout << "Enter the name you wish to give the file: ";
	getline(cin, fileName);
	myFile.open(fileName);
	if (myFile.is_open())
	{
		graph->breadthFirstTraversal(writeFile, myFile);
		pause();
	}
	
}

bool breadthTraversal(Prim<string>* graph)
{
	if (graph->getNumVertices() == 0)
	{
		cout << "ERROR: Graph does not contain enough vertices to traverse" << endl;
		return false;
	}
	else if (graph->getNumEdges() < graph->getNumVertices() - 1)
	{
		cout << "ERROR: Graph does not contain enough edges to traverse" << endl;
		return false;
	}
	else
	{
		graph->breadthFirstTraversal(display);
		return true;
	}

}

bool depthTraversal(Prim<string>* graph)
{
	if (graph->getNumVertices() == 0)
	{
		cout << "ERROR: Graph does not contain enough vertices to traverse" << endl;
		return false;
	}
	else if (graph->getNumEdges() < graph->getNumVertices() - 1)
	{
		cout << "ERROR: Graph does not contain enough edges to traverse" << endl;
		return false;
	}
	else
	{
		graph->depthFirstTraversal(display);
		return true;
	}
}
int main()
{
	LoopingMenu debugMenu;
	vector<string> strDebugMenu{"Debug/Testing Menu", "Test file reading", "Run Prim graph test", "Read from file and test", "Test Traversal", "Traverse using first item of graph", "Exit Menu"};
	vector<my_func> debugMenuFunctions{
		[]()	// Debug menu option 1
		{
			cout << "Testing reading a file into a graph" << endl;
			ifstream inFile;

			if (openInputFile(inFile))
			{
				Prim<string>* graph = new Prim< string > ;
				readFileIntoGraph(graph, inFile);
				cout << "Testing breadthFirstTraversal" << endl;
				graph->breadthFirstTraversal("2227.83.27.247", display);
				pause();
				cout << "Testing deapthFirstTraversal" << endl;
				graph->depthFirstTraversal("2227.83.27.247", display);
				pause();
			}
			else
				return;
		},	// End debug menu option 1
		[]()		// Debug menu option 2
		{
		//	cout << "In debug menu option 2" << endl;
			Prim<string>* graph = new Prim< string >;
			cout << "Running graph test" << endl;
			primTest(graph);
			pause();
		},	// End debug menu option 2
		[]()	// Debug menu option 3
		{
			cout << "Reading a file into a graph" << endl;
			ifstream inFile;

			if (openInputFile(inFile))
			{
				Prim<string>* graph = new Prim< string >;
				readFileIntoGraph(graph, inFile);
				cout << "Running graph test" << endl;
				primFileTest(graph);
			}
			else
				cout << "Failed to open file" << endl;
		},	// End debug menu option 3
		[]()	// Debug menu option 4
		{
			ifstream inFile;
			if (openInputFile(inFile))
			{
				Prim<string>* graph = new Prim < string > ;
				readFileIntoGraph(graph, inFile);
				vector<string> vect;
		//		graph->traverseAll(visitAddToVector, vect);
				for (unsigned int i = 0; i < vect.size(); i++)
					cout << i+1 << ") - " << vect[i] << endl;
				// for (all items in vector)
				//	cout << i+1) - << vector[i];
				// cout << "select item number"
				pause();
			}
			else
				cout << "Failed to open file" << endl;
		},	// End debug menu option 4
		[]()	// Debug menu option 5
		{
			ifstream inFile;
			if (openInputFile(inFile))
			{
				Prim<string>* graph = new Prim < string > ;
				readFileIntoGraph(graph, inFile);
		//		string firstItem = graph->getFirstVertex();
		//		graph->depthFirstTraversal(firstItem, display);
			}
			else
				cout << "Failed to open file" << endl;
			pause();
		},	// End debug menu option 5
		[](){cout << "In debug menu option 4" << endl; }		// Debug menu option 6
	};	
	debugMenu.setMenu(strDebugMenu, debugMenuFunctions);
	debugMenu.runMenu();

	Prim<string>* mainGraph = new Prim<string>;

	LoopingMenu mainMenu;
	LoopingMenu modifyMenu;
	LoopingMenu displayMenu;
	LoopingMenu addEdgeMenu;

	vector<string> strAddEdgeMenu{ "MAIN MENU -> MODIFY DATA -> ADD AN EDGE", "Add an edge using existing vertices", "Add an edge using new vertices", "Add an edge using an existing vertex and a new vertex", "Go back" };
	// Vectors of lambdas (essentially really cool in line functions) to use for the menus
	// Can use vector<my_func> instead of vector<function<void()>>
	// Instead of creating individual lambda functions and adding them to the vector manually, create a vector of lambda functions using an initialization list
	// This makes for simpler, easier to update code, but the functions being part of an initialization list can make it more difficult to read.
	vector<my_func> mainMenuFunctions{
		[&]() // Main menu option 1	(Read file into graph)
		{
			cout
				<< "================================================================================"
				<< "\tMAIN MENU -> READ FROM FILE" << endl
				<< "================================================================================";
			readFileIntoGraph(mainGraph);
			pause();
		},	// End main menu option 1 (Read file into graph)
		[&](){ modifyMenu.runMenu(); },	// Main menu option 2	(Modify graph data)
		[&](){ displayMenu.runMenu(); },	// Main menu option 3	(Display graph data)
		[&]()	// Main menu option 4 (Solve minimum spanning tree)
		{ 
			cout
				<< "================================================================================"
				<< "\tMAIN MENU -> MINMUM SPANNING TREE" << endl
				<< "================================================================================";
			if (mainGraph->getNumVertices() < 1 || (mainGraph->getNumEdges() < mainGraph->getNumVertices() - 1))
				cout << "ERROR: Not enough data for a minimum spanning tree" << endl;
			else
			{
				mainGraph->createMinSpanTree();
				mainGraph->writeMinSpanTree(cout);
				pause();
				cout << "Write tree to a file? Y/N: " << endl;
				// ofstream file
				// openOutputFile(file)
				// graph->writeToFile();
			}

			pause();
		},		// End main menu option 4	(Solve minimum spanning tree)
		[](){ cout << "In Main Menu option 5" << endl; },		// Main menu option 5	(Info and help)
		[](){}		// Main menu option 6	(Exit program)
	};
	
	
	// Vector of lambda functions for the "Modify graph data" sub menu
	vector<my_func> modifyMenuFunctions{
		[&](){ addEdgeMenu.runMenu(); }, // End modify menu option 1	(Add an edge to the graph)
		[&]()	// Modify menu option 3 (Remove an edge from the graph)
		{ 
			cout
				<< "================================================================================"
				<< "\tMAIN MENU -> MODIFY DATA -> REMOVE EDGE" << endl
				<< "================================================================================";
			// Need to display all edges, and allow user to select which edge to remove
			// use remove function
			mainGraph->remove();
			pause();
		},	// End modify menu option 2 (Remove an edge from the graph)
		[&]()	// Modify menu option 3 (Undo action)
		{
			cout
				<< "================================================================================"
				<< "\tMAIN MENU -> MODIFY DATA -> UNDO" << endl
				<< "================================================================================";
			mainGraph->undo();
			pause();
		},	// End modify menu option 3	(Undo action)
		[](){}		// Modify menu option 4	(Go back)
	};
	// Vector of lambda functions for the "Display graph data" sub menu
	vector<my_func> displayMenuFunctions{
		[&]()	// Display menu option 1	(Display on screen using depth-first traversal)
		{ 
			cout
				<< "================================================================================"
				<< "\tMAIN MENU -> DISPLAY GRAPH DATA -> DEPTH-FIRST TRAVERSAL" << endl
				<< "================================================================================";
			depthTraversal(mainGraph);
			pause();
		},	// End display menu option 1 (Display on screen using depth-first traversal)
		[&]()	// Display menu option 2	(Display on screen using breadth-first traversal)
		{
			cout
				<< "================================================================================"
				<< "\tMAIN MENU -> DISPLAY GRAPH DATA -> BREADTH-FIRST TRAVERSAL" << endl
				<< "================================================================================";
			breadthTraversal(mainGraph);
			pause();
		},	// End display menu option 2 (Display on screen using breadth-first traversal)
		[&]()
		{ 
			cout << "In display menu option 3" << endl; 
			writeToTextFile(mainGraph);
			pause();
		},	// Display menu option 3	(Write to a text file using breadth-first traversal)
		[](){}		// Display menu option 4	(Go back)
	};
	vector<my_func> addEdgeMenuFunctions
	{
		[&](){ mainGraph->addUsingExisting(); },	// Add edge menu option 1 (Add an edge using existing vertices),	// End add edge menu option 1 (Add an edge using existing vertices)
		[&](){ mainGraph->addUsingNew(); },	// Add edge menu option 2 (Add an edge using new vertices)	// End add edge menu option 2 (Add an edge using new vertices)
		[&](){ mainGraph->addUsingExistingAndNew(); },	// Add edge menu option 3 (Add an edge using an existing vertex and a new vertex)	// End add edge menu option 3 (Add an edge using an existing vertex and a new vertex)
		[](){}	// Add edge menu option 4 (Go back)
	};

	mainMenu.setMenu(strMainMenu, mainMenuFunctions);
	modifyMenu.setMenu(strModifyMenu, modifyMenuFunctions);
	displayMenu.setMenu(strDisplayMenu, displayMenuFunctions);
	addEdgeMenu.setMenu(strAddEdgeMenu, addEdgeMenuFunctions);

	mainMenu.runMenu();
	
	/*
	
	LinkedGraph<string>* myGraph = new LinkedGraph<string>();
	cout << "Testing Graph . . . ." << endl << endl ;
	graphTest(myGraph);	
	*/

	//menuTester();

	delete mainGraph;


	cout << "End of program" << endl;
	pause();
	return 0;
}  // end main