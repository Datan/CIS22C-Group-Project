#ifndef _MENU
#define _MENU

#include "MenuInterface.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

typedef function<void()> my_func;

class Menu : public MenuInterface
{
protected:	
	// Data memebers
	string header;
	vector<string> menuVector;
	vector<my_func> functions;

	// Private functions
	int choice(int low,int high);
	bool openInputFile(ifstream & ifs, string fileName);
	void readFile(ifstream &ifs);
	bool setInputFile(string fileName);
	void displayMenu();
	void displayHeader();
	void displayMenuBody();
	int makeChoice();

public:
	Menu();
	Menu(string fileName, vector<my_func> & funcs);
	Menu(vector<string> & menu, vector<my_func> & funcs);
	virtual void setMenu(string fileName, const vector<my_func> & funcs);
	virtual void setMenu(vector<string> & fullMenu, vector<my_func> & funcs);
	virtual void runMenu();
	virtual void clear();

	void operator=(const Menu & menu);
};
Menu::Menu()	// Default constructor
{
	header = "NULL";
}

Menu::Menu(string fileName, vector<my_func> & funcs)
{
	this->setInputFile(fileName);
	functions = funcs;
}

Menu::Menu(vector<string> & fullMenu, vector<my_func> & funcs)	// Constructor
{
	header = fullMenu[0];
	for (unsigned int i = 1; i < fullMenu.size(); i++)
		menuVector.push_back(fullMenu[i]);
	functions = funcs;
}
// ********** PRIVATE FUNCTIONS ********** //

int Menu::choice(int low, int high)
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

void Menu::readFile(ifstream & ifs)
{
	string temp;
	getline(ifs, temp);
	header = temp;
	while (getline(ifs, temp))
	{
		menuVector.push_back(temp);
	}
}

	/** Initializes the header and body of a menu by reading the values from a file	
		@return True if the file was successfully opened and the values assigned, false otherwise	*/
bool Menu::setInputFile(string fileName)
{
	ifstream inFile;
	inFile.open(fileName);
	if (inFile.is_open())
	{
		readFile(inFile);
		return true;
	}
	else
		return false;
}

void Menu::displayMenu()
{
	system("cls");
	cout << "================================================================================";
	displayHeader();
	cout << endl << "================================================================================";
	cout << "\tWhat would you like to do?" << endl;
	displayMenuBody();
}

void Menu::displayHeader()
{
	cout << "\t" << header;
}

void Menu::displayMenuBody()
{
	for (unsigned int i = 0; i < menuVector.size(); i++)
	{
		cout << "\t" << i + 1 << ") - " << menuVector[i] << endl;
	}
}

/** Allows the user to make a choice corresponding to the menu
@return A number between 1-n, corresponding to the index of possible choices. Where 1 is the first possible choice, and n is the last	*/
int Menu::makeChoice()
{
	if (menuVector.size() > 0)
		return choice(1, menuVector.size());
	else
		return 0;
}

// ********** PUBLIC FUNCTIONS ********** //



void Menu::setMenu(string fileName, const vector<my_func> & funcs)
{
	if (setInputFile(fileName))
	{
		functions = funcs;
		if (menuVector.size() != functions.size())	// Every menu option must have a function to go with it. If the vectors are not of equal size, then the program would run into an error
		{
			cout << "ERROR: There must be a single function for every menu option" << endl;	// Consider throwing an exception instead of displaying an error
			return;
		}
	}
	else
		cout << "ERROR: Failed to open file" << endl;
}

void Menu::setMenu(vector<string> & fullMenu, vector<my_func> & funcs)
{
	header = fullMenu[0];
	for (unsigned int i = 1; i < fullMenu.size(); i++)
		menuVector.push_back(fullMenu[i]);
	functions = funcs;
}
	/** Runs a menu.
		Should not only display the menu, but allow the user to interact with the menu, as well as execute all choices made. note: Executing choices may not be possible
		@pre The menu should have a header, body, and valid choices
	*/
void Menu::runMenu()
{
	if (menuVector.size() != functions.size())	// Every menu option must have a function to go with it. If the vectors are not of equal size, then the program would run into an error
	{
		cout << "ERROR: Not enough functions for menu options" << endl;	// Consider throwing an exception instead of displaying an error
		return;
	}
	system("cls");
	displayMenu();
	int i = this->makeChoice();	// Get the index of the menu choice, where 1 corresponds to the first choice
	system("cls");
	// Add comparison to make sure the menu choice is still within the vector indexes
	functions[i - 1]();		// Index of function corresponding to an menu choice is 1 less than the index of the menu choice. (The function for menu choice 1 has an index of 0)
}

void Menu::clear()
{
	header = "NULL";
	menuVector.clear();
	functions.clear();
}

void Menu::operator=(const Menu & menu)
{
	header = menu.header;
	menuVector = menu.menuVector;
	functions = menu.functions;
}

#endif