//  Created by Frank M. Carrano and Tim Henry.
//  Copyright (c) 2013 __Pearson Education__. All rights reserved.

#include <iostream>
#include <string>
#include "LinkedGraph.h"
#include "LoopingMenu.h"
using namespace std;

typedef function<void()> my_func;

const string strMainMenu = "Main Menu.txt";
const string strModifyMenu = "Modify Menu.txt";
const string strDisplayMenu = "Display Menu.txt";

vector<string> testGlobalMenu{"Global Test Header", "Use function 1", "Use function 2", "Use function 3", "Exit Menu"};

void display(string& anItem)
{
   cout << "Displaying item - " << anItem << endl;
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
  
   cout << "\nDepth-first traversal (should be A B E F J C G K L D H M I N):" << endl;
   testGraph->depthFirstTraversal(A, display);
  
   cout << "\nBreadth-first traversal (should be A B C D E F G H I J K L M N):" << endl;
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
	cout << endl;
	ifs.open(filename.c_str());
	return ifs.is_open();
}

void readFileIntoGraph(LinkedGraph<string>* graph, ifstream &inFile)
{
	if (!inFile || !inFile.is_open())
		return;
	if (inFile.eof())
	{
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
	}
	string strTemp;
	vector<string> vertexVect;
	while (strTemp != "BREAK")
	{
		inFile >> strTemp;
		vertexVect.push_back(strTemp);
	}
	string strStart, strEnd;
	double dWeight;
	int iWeight;
	while (inFile >> dWeight >> strStart >> strTemp >> strEnd)
	{
		iWeight = static_cast<int> (dWeight * 100);
		graph->add(strStart, strEnd, iWeight);
	}
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


	Menu* testMenu = new LoopingMenu(strVec, functions);
	testMenu->runMenu();
	system("pause");

	testMenu->clear();
	testMenu->setMenu(testGlobalMenu, functions);
	testMenu->runMenu();
	system("pause");

	testMenu->clear();
	testMenu->setMenu("Test Menu.txt", functions);
	testMenu->runMenu();
	system("pause");
}
int main()
{
	Menu* mainMenu;
	Menu* modifyMenu;
	Menu* displayMenu;

	mainMenu = new LoopingMenu;
	modifyMenu = new LoopingMenu;
	displayMenu = new LoopingMenu;

	// Vectors of lambdas (essentially really cool in line functions) to use for the menus
	// Can use vector<my_func> instead of vector<function<void()>>
	// Instead of creating individual lambda functions and adding them to the vector manually, create a vector of lambda functions using an initialization list
	// This makes for simpler, easier to update code, but the functions being part of an initialization list can make it more difficult to read.
	vector<my_func> modifyMenuFunctions{
		[](){ cout << "In modify menu option 1" << endl; },		// Modify menu option 1
		[](){ cout << "In modify menu option 2" << endl; },		// Modify menu option 2
		[](){ cout << "In modify menu option 3" << endl; },		// Modify menu option 3
		[](){ cout << "In modify menu option 4" << endl; },		// Modify menu option 4
		[](){ cout << "In modify menu option 5" << endl; }		// Modify menu option 5
	};
	vector<my_func> displayMenuFunctions{
		[](){ cout << "In display menu option 1" << endl; },	// Display menu option 1
		[](){ cout << "In display menu option 2" << endl;},		// Display menu option 2
		[](){ cout << "In display menu option 3" << endl; },	// Display menu option 3
		[](){ cout << "In display menu option 4" << endl; }		// Display menu option 4
	};
	vector<my_func> mainMenuFunctions{
		[]() { cout << "In Main Menu option 1" << endl; },		// Main menu option 1
		[&](){ cout << "In Main Menu option 2" << endl; modifyMenu->runMenu(); },	// Main menu option 2
		[&](){ cout << "In Main Menu option 3" << endl; displayMenu->runMenu(); },	// Main menu option 3
		[](){ cout << "In Main Menu option 4" << endl; },		// Main menu option 4
		[](){ cout << "In Main Menu option 5" << endl; }		// Main menu option 5
	};

	mainMenu->setMenu(strMainMenu, mainMenuFunctions);
	modifyMenu->setMenu(strModifyMenu, modifyMenuFunctions);
	displayMenu->setMenu(strDisplayMenu, displayMenuFunctions);

	mainMenu->runMenu();
	
	/*
	
	LinkedGraph<string>* myGraph = new LinkedGraph<string>();
	cout << "Testing Graph . . . ." << endl << endl ;
	graphTest(myGraph);	
	*/

	//menuTester();

	delete mainMenu;
	delete modifyMenu;
	delete displayMenu;

	cout << endl << "End of program" << endl;
	system("pause");
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
