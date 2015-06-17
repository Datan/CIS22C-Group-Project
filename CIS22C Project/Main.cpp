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

vector<string> testGlobalMenu{"Global Test Header", "Use function 1", "Use function 2", "Use function 3", "Exit Menu"};	// Can't make it const without changing the parameters for the Menu classes

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
		cin.ignore(10000, '\n');
		cin.clear();
		if (cI < low || cI > high)
			cout << "Invalid choice, try again" << endl;
	} while (cI < low || cI > high);
	return cI;
}
template<typename ItemType>
void display(ItemType& anItem)
{
   cout << "Displaying item - " << anItem << endl;
}

template<typename ItemType>
void visitAddToVector(ItemType & anItem, vector<ItemType> &vect)	
{
	vect.push_back(anItem);
	// To be used as the vist function for depthFirstTraversal and breadthFirstTraversal
	// Will have to overload those functions so that they can accept a visit function with a vector<string> parameter
	// Will visit each vertex, and add that vertex to a vector
}
void AddEdge(Prim<string>* graph, vector<string> &vecVertices)	
{
	// Uses the vecVertices to store a list of all vertices in the graph
	// Display a list of all vertices
		// Let user choose one vertex
	// Display a list of all vertices excluding the chosen one
		// Let user choose one vertex
	// User enters a weight for the new edge between vertices
	// Call Prim add function
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
	ifs.open(filename.c_str());
	return ifs.is_open();
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
	vector<string> vecVertices;	// Will be used to test if all the vertices from the file exist in the graph
	while (strTemp != "BREAK")
	{
		inFile >> strTemp;
		vecVertices.push_back(strTemp);
	}
	
//	Prim<string>* tempPrim = new Prim < string > ;	Will be used to initially read, then will be copied to graph if conditions are met
	string strStart, strEnd;
	char cTemp;
	float dWeight;
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

void menuTester()
{
	string temp = "item";
	int val = 5;
	function<void()> function1 = []() {cout << "This is function 1" << endl; };
	function<void()> function2 = [&]() {cout << "This is function 2 \nNow using function1" << endl; function1(); };
	auto function3 = [&]() {cout << "This is function 3\nUsing display function: " << endl; display(temp); };
	auto function4 = []() { cout << "This is function 4" << endl; };



	vector<string> strVec { "In-line Test Header", "Use function 1", "Use function 2", "Use function 3", "Exit Menu" };

	vector<function<void()>> functions;
	
	functions.push_back(function1);
	functions.push_back(function2);
	functions.push_back(function3);
	functions.push_back([]() {cout << "This is function 4" << endl; });
//	functions.push_back(function4);


	Menu* testMenu = new Menu(strVec, functions);
	testMenu->runMenu();
	pause();

	testMenu->clear();
	testMenu->setMenu(testGlobalMenu, functions);
	testMenu->runMenu();
	pause();

	testMenu->clear();
	testMenu->setMenu("Test Menu.txt", functions);
	testMenu->runMenu();
	pause();
}
int main()
{
	Prim<string>* mainGraph = new Prim<string>;
	LoopingMenu debugMenu;
	vector<string> strDebugMenu{"Debug/Testing Menu", "Test file reading", "Run Prim graph test", "Read from file and test", "Test Traversal", "Exit Menu"};
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
		[](){
		//	cout << "In debug menu option 2" << endl;
			Prim<string>* graph = new Prim< string >;
			cout << "Running graph test" << endl;
			primTest(graph);
			pause();
		},		// Debug menu option 2
		[](){
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
				return;
		},		// Debug menu option 3
		[]()
		{
			ifstream inFile;
			if (openInputFile(inFile))
			{
				Prim<string>* graph = new Prim < string > ;
				readFileIntoGraph(graph, inFile);
				vector<string> vect;
				graph->traverseAll(visitAddToVector, vect);
				for (unsigned int i = 0; i < vect.size(); i++)
					cout << i+1 << ") - " << vect[i] << endl;
				// for (all items in vector)
				//	cout << i+1) - << vector[i];
				// cout << "select item number"
				pause();
			}
		},		// Debug menu option 4
		[](){cout << "In debug menu option 4" << endl; }		// Debug menu option 5
	};
	debugMenu.setMenu(strDebugMenu, debugMenuFunctions);
	debugMenu.runMenu();

	LoopingMenu mainMenu;
	LoopingMenu modifyMenu;
	LoopingMenu displayMenu;

	// Vectors of lambdas (essentially really cool in line functions) to use for the menus
	// Can use vector<my_func> instead of vector<function<void()>>
	// Instead of creating individual lambda functions and adding them to the vector manually, create a vector of lambda functions using an initialization list
	// This makes for simpler, easier to update code, but the functions being part of an initialization list can make it more difficult to read.
	vector<my_func> mainMenuFunctions{
		[&]() // Main menu option 1	(Read file into graph)
		{
			cout
				<< "================================================================================"
				<< "\tFile Reading" << endl
				<< "================================================================================";
			ifstream ifs;
			if (openInputFile(ifs))
			{
				readFileIntoGraph(mainGraph, ifs);
				cout << "File reading successful" << endl;
			}		
			else
				cout << "ERROR: Failed to read from file" << endl;
			ifs.close();
			ifs.clear();
			pause();
		},	// End main menu option 1
		[&](){ modifyMenu.runMenu(); },	// Main menu option 2	(Modify graph data)
		[&](){ displayMenu.runMenu(); },	// Main menu option 3	(Display graph data)
		[&](){ cout << "In Main Menu option 4" << endl; mainGraph->createMinSpanTree(); mainGraph->writeMinSpanTree(cout); },		// Main menu option 4	(Solve minimum spanning tree)
		[](){cout << "In Main Menu option 5" << endl; },		// Main menu option 5	(Info and help)
		[](){ cout << "In Main Menu option 6" << endl; }		// Main menu option 6	(Exit program)
	};

	vector<my_func> modifyMenuFunctions{
		[](){ cout << "In modify menu option 1" << endl; },		// Modify menu option 1
		[](){ cout << "In modify menu option 2" << endl; },		// Modify menu option 2
		[](){ cout << "In modify menu option 3" << endl; },		// Modify menu option 3
		[](){ cout << "In modify menu option 4" << endl; },		// Modify menu option 4
		[](){ cout << "In modify menu option 5" << endl; }		// Modify menu option 5
	};
	vector<my_func> displayMenuFunctions{
		[&]()	// Display menu option 1	(Display on screen using depth-first traversal)
		{ 
			cout
				<< "================================================================================"
				<< "\tDepth-First Traversal" << endl
				<< "================================================================================";
			if (mainGraph->getNumVertices() == 0)
			{
				cout << "ERROR: Graph does not contain enough vertices to traverse" << endl;
			}
			else
			{
				vector<string> vectVertices;
				mainGraph->traverseAll(visitAddToVector, vectVertices);
				cout << "Choose an initial vertex for traversal" << endl;
				for (unsigned int i = 0; i < vectVertices.size(); i++)
					cout << i + 1 << ") - " << vectVertices[i] << endl;
				int indexChoice;	
				cout << "Enter the initial vertex for traversal: ";
				indexChoice = choice(1, vectVertices.size());	// Get index for user choice from list
				system("cls");
				cout
					<< "================================================================================"
					<< "\tDepth-First Traversal" << endl
					<< "================================================================================";
				mainGraph->depthFirstTraversal(vectVertices[indexChoice-1], display);
			}
			pause();
		},	// End display menu option 1
		[&]()	// Display menu option 2	(Display on screen using breadth-first traversal)
		{
			cout
				<< "================================================================================"
				<< "\tBreadth-First Traversal" << endl
				<< "================================================================================";
			if (mainGraph->getNumVertices() == 0)
			{
				cout << "ERROR: Graph does not contain enough vertices to traverse" << endl;
			}
			else
			{
				vector<string> vectVertices;
				mainGraph->traverseAll(visitAddToVector, vectVertices);
				cout << "Choose an initial vertex for traversal" << endl;
				for (unsigned int i = 0; i < vectVertices.size(); i++)
					cout << i + 1 << ") - " << vectVertices[i] << endl;
				int indexChoice;
				cout << "Enter the initial vertex for traversal: ";
				indexChoice = choice(1, vectVertices.size());	// Get index for user choice from list
				system("cls");
				cout
					<< "================================================================================"
					<< "\tBreadth-First Traversal" << endl
					<< "================================================================================";
				mainGraph->breadthFirstTraversal(vectVertices[indexChoice-1], display);
			}
			pause();
		},	// End display menu option 2
		[](){ cout << "In display menu option 3" << endl; },	// Display menu option 3	(Write to a text file using breadth-first traversal)
		[](){ cout << "In display menu option 4" << endl; }		// Display menu option 4	(Go back)
	};

	mainMenu.setMenu(strMainMenu, mainMenuFunctions);
	modifyMenu.setMenu(strModifyMenu, modifyMenuFunctions);
	displayMenu.setMenu(strDisplayMenu, displayMenuFunctions);

	mainMenu.runMenu();
	
	/*
	
	LinkedGraph<string>* myGraph = new LinkedGraph<string>();
	cout << "Testing Graph . . . ." << endl << endl ;
	graphTest(myGraph);	
	*/

	//menuTester();

	delete mainGraph;
	cout << endl << "End of program" << endl;
	pause();
	return 0;
}  // end main

/*
 Testing Graph . . . .
 
 Vertices :14
 Edges :14
 
 Depth-first traversal (should be A B E F J C G K L D H M I N):
 Displaying item - A
 Displaying item - B
 Displaying item - E
 Displaying item - F
 Displaying item - J
 Displaying item - C
 Displaying item - G
 Displaying item - K
 Displaying item - L
 Displaying item - D
 Displaying item - H
 Displaying item - M
 Displaying item - I
 Displaying item - N
 
 Breadth-first traversal (should be A B C D E F G H I J K L M N):
 Displaying item - A
 Displaying item - B
 Displaying item - C
 Displaying item - D
 Displaying item - E
 Displaying item - F
 Displaying item - G
 Displaying item - H
 Displaying item - I
 Displaying item - J
 Displaying item - K
 Displaying item - L
 Displaying item - M
 Displaying item - N
*/
