#ifndef _LOOPING_MENU
#define _LOOPING_MENU

#include "Menu.h"

using namespace std;

typedef function<void()> my_func;

class LoopingMenu : public Menu
{
private:
	bool exitFlag;
public:
	LoopingMenu();
	LoopingMenu(string fileName, vector<my_func> & funcs);
	LoopingMenu(vector<string> & fullMenu, vector<my_func> & funcs);
	virtual void runMenu();
	virtual void clear();
};

LoopingMenu::LoopingMenu() : Menu(){ exitFlag = false; }
LoopingMenu::LoopingMenu(string fileName, vector<my_func> & funcs) : Menu(fileName, funcs){ exitFlag = false; }
LoopingMenu::LoopingMenu(vector<string> & fullMenu, vector<my_func> & funcs) : Menu(fullMenu, funcs){ exitFlag = false; }

void LoopingMenu::runMenu()
{
	if (menuVector.size() != functions.size())	// Every menu option must have a function to go with it. If the vectors are not of equal size, then the program would run into an error
	{
		cout << "ERROR: Not enough functions for menu options" << endl;	// Consider throwing an exception instead of displaying an error
		return;
	}
	exitFlag = false;
	while (!exitFlag)
	{
		system("cls");
		displayMenu();
		int i = this->makeChoice();	// Get the index of the menu choice, where 1 corresponds to the first choice
		if (i == functions.size())	// This assumes that the last choice is ALWAYS the one that quits the looping menu
			exitFlag = true;
		// Add comparison to make sure the menu choice is still within the vector indexes
		functions[i - 1]();		// Index of function corresponding to an menu choice is 1 less than the index of the menu choice. (The function for menu choice 1 has an index of 0)
	}
}

void LoopingMenu::clear()
{
	this->Menu::clear();
	exitFlag = false;
}
#endif