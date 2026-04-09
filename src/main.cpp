#include <iostream>
#include "../include/display.hpp"
#include "../include/dictionary.hpp"

int main() {
	std::cout << "Welcome to T-type..!!" << std::endl;

	clearScreen();

	std::cout << "-- dictionary test --" << std::endl;

	Dictionary dict;

	// show initial words
	dict.showWords();
	std::cout << "Total words in dictionary: " << dict.getWordCount() << std::endl;

	// add new words
	dict.addWord("alahamdulillah");
	dict.addWord("Hey bhagwan");
	dict.addWord("Jesus christ");
	dict.addWord("waheguru ji");

	// show updated words.. 
	std::cout << "\nAfter adding new words:" << std::endl;
	dict.showWords();
	std::cout << "Words count : " << dict.getWordCount() << std::endl;


	std::cout << "Press Enter to continue...";
	std::cin.get();

	return 0;
}
