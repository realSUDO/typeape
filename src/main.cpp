#include <iostream>
#include "../include/display.hpp"

int main() {
	std::cout << "Welcome to T-type..!!" << std::endl;

	clearScreen();

	std::cout << "\n --- testing colors\n\n " ;

	std::string word = "Hello";
	setGrayColor();
	std::cout << "Grey text : " << word << std::endl;
	setWhiteColor();
	std::cout << "White text : " << word << std::endl;
	resetColor();
	std::cout << "Reset color : " << word << std::endl;

	// testing the function.. 
	std::cout << "\n --- testing printColoredText function\n\n " ;
	printColoredText("This is gray text.\n");
	printColoredText("This is white text.\n", true);
	resetColor();


	std::cout << "Press Enter to continue...";
	std::cin.get();

	return 0;
}
