#include "../include/display.hpp"

#include <iostream>

// terminal colors.. 
const std::string GRAY_COLOR = "\033[90m";
const std::string WHITE_COLOR = "\033[97m";
const std::string RESET_COLOR = "\033[0m";


void clearScreen() {
	#ifdef _WIN32
		system("cls"); // windows.. ew
	#else
		system("clear"); // unix
	#endif 
}

void setGrayColor() {
	std::cout << GRAY_COLOR;
}


void setWhiteColor() {
	std::cout << WHITE_COLOR;
}

void resetColor() {
	std::cout << RESET_COLOR;
}

void printColoredText(const std::string& text,bool isTyped) {
	if (isTyped) {
		setWhiteColor();
	} else {
		setGrayColor();
	}
	std::cout << text;
	resetColor();
}
