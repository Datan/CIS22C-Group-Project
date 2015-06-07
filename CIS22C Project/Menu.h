#include "MenuInterface.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

class Menu : public MenuInterface
{
private:	
	// Data memebers
	string header;
	vector<string> menuVector;

	// Private functions
	char choice(char low, char high);
	int choice(int low, int high);
	bool openInputFile(ifstream & ifs, string fileName);
	void readFile(ifstream &ifs);
	void displayHeader();
	void displayMenuBody();

public:
	Menu();
	Menu(string head, vector<string> menu);
	bool setInputFile(string fileName);
	virtual void runMenu();
	void displayMenu();
	int makeChoice();

	friend ifstream& operator<<(ifstream & ifs, Menu & menu);
	friend fstream& operator<<(fstream & fs, Menu & menu);
};

// ********** PRIVATE FUNCTIONS ********** //

	/** Opens an input file with a given name
		@post A file is open
		@param &ifs Address of the ifstream object being used to open the file
		@param fileName Name of the file to open
		@return True if the file was successfully opened, false otherwise	*/
bool Menu::openInputFile(ifstream & ifs, string fileName)
{
	ifs.open(fileName);
	return ifs.is_open();
}

char Menu::choice(char low, char high)
{	/*
	Function: Allows user to input char values between two values, based on ASCII values
	Parameters:
	low - Lowest value char to be used
	high - Highest char value to be used
	Returns: Valid character input (characters between 'low' and 'high' based on ASCII values)
	*/
	char cI;
	do			// Loop runs until valid character input is received (characters between 'low' and 'high')
	{
		cout << "Choice: ";
		cin.get(cI);
		//cin.clear();
		cin.ignore(10000, '\n');
		if (cI < low || cI > high)
			cout << "Invalid choice, try again" << endl;
	} while (cI < low || cI > high);
	return cI;
}

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

void Menu::displayHeader()
{
	cout << "\t\t\t" << header;
}
void Menu::displayMenuBody()
{
	for (int i = 0; i < menuVector.size(); i++)
	{
		cout << "\t" << i + 1 << ") - " << menuVector[i] << endl;
	}
}
// ********** PUBLIC FUNCTIONS ********** //
Menu::Menu()	// Default constructor
{
	header = "NULL";
}

Menu::Menu(string head, vector<string> menu)	// Constructor
{
	header = head;
	menuVector = menu;
}

	/** Initializes the header and body of a menu by reading the values from a file	
		@return True if the file was successfully opened and the values assigned, false otherwise	*/
bool Menu::setInputFile(string fileName)
{
	ifstream inFile;
	if (this->openInputFile(inFile, fileName))
	{
		readFile(inFile);
	}
	else
		return false;
}



	/** Runs a menu.
		Should not only display the menu, but allow the user to interact with the menu, as well as execute all choices made. note: Executing choices may not be possible
		@pre The menu should have a header, body, and valid choices
	*/
void Menu::runMenu()
{
	displayMenu();
	switch (this->makeChoice())
	{

	}
}

	/** Displays the header and body of a menu
	*/
void Menu::displayMenu()
{
	cout << "================================================================================";
	displayHeader();
	cout << "================================================================================";
	displayMenuBody();
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




ifstream& operator<<(ifstream & ifs, Menu & menu)
{
	string temp;
	getline(ifs, temp);
	menu.header = temp;
	while (getline(ifs, temp))
	{
		menu.menuVector.push_back(temp);
	}

}

fstream& operator<<(fstream & fs, Menu & menu)
{
	string temp;
	getline(fs, temp);
	menu.header = temp;
	while (getline(fs, temp))
	{
		menu.menuVector.push_back(temp);
	}
}
