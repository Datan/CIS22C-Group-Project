#include <fstream>
using namespace std;

class MenuInterface
{
public:
	MenuInterface(){};

	/** Runs a menu.
	Should not only display the menu, but allow the user to interact with the menu, as well as execute all choices made. 	*/
	virtual void runMenu() = 0;	// note: Executing choices may not be possible, this function may not be used

	/** Displays the header and body of a menu	*/
	virtual void displayMenu() = 0;

	/** Allows the user to make a choice corresponding to the menu	
		@return A number between 1-n, corresponding to the index of possible choices. Where 1 is the first possible choice, and n is the last	*/
	virtual int makeChoice() = 0;

	/** Initializes the header and body of a menu by reading the values from a file	
		@return True if the file was successfully opened and the values assigned, false otherwise	*/
	virtual bool setInputFile(string fileName) = 0;

private:

};