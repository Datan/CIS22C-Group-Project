#ifndef _LOOPING_MENU
#define _LOOPING_MENU

#include "Menu.h"

using namespace std;

class LoopingMenu : public Menu
{
public:
	virtual void runMenu();
};
void LoopingMenu::runMenu()
{
	if (menuVector.size() != functions.size())	// Every menu option must have a function to go with it. If the vectors are not of equal size, then the program would run into an error
	{
		cout << "ERROR: Not enough functions for menu options" << endl;	// Consider throwing an exception instead of displaying an error
		return;
	}
	while (true)	// Infinite loop. Will loop until the user chooses the last menu option. This assumes that the last choice is ALWAYS the one that quits the looping menu
	{
		system("cls");
		displayMenu();
		int i = this->makeChoice();	// Get the index of the menu choice, where 1 corresponds to the first choice
		system("cls");
		// Add comparison to make sure the menu choice is still within the vector indexes
		functions[i - 1]();		// Index of function corresponding to an menu choice is 1 less than the index of the menu choice. (The function for menu choice 1 has an index of 0)
		if (i == functions.size())	// This assumes that the last choice is ALWAYS the one that quits the looping menu
			return;
	}
}

#endif