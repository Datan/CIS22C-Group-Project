#ifndef _MENU_INTERFACE
#define _MENU_INTERFACE

#include <string>
#include <vector>
#include <functional>

using namespace std;

class MenuInterface
{
public:
	/** Runs a menu.
	Should not only display the menu, but allow the user to interact with the menu, as well as execute all choices made. 	*/
	virtual void runMenu() = 0;	// note: Executing choices may not be possible, this function may not be used

	virtual void setMenu(string filename, const vector<function<void()>> & funcs) = 0;

	virtual void setMenu(vector<string> & fullMenu, vector<function<void()>> & funcs) = 0;

	virtual void clear() = 0;
};

#endif