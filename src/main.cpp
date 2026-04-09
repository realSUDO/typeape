#include <iostream>
#include "../include/display.hpp"
#include "../include/dictionary.hpp"

int main() {
	std::cout << "Welcome to T-type..!!" << std::endl;

	clearScreen();

	std::cout << "-- random words.. test --" << std::endl;

	Dictionary dict;

	// show initial words
	dict.showWords();

	std::cout << "\n -- random words.. -- " ; 
	for (int i = 0; i < 5; ++i) {
		std::string randomWord = dict.getRandomWord();
		std::cout << "Random Word " << (i + 1) << ": " << randomWord << std::endl;
	}

	std::cout << "\n -- random sentence.. -- " << std::endl;
	for (int i = 0; i < 3; ++i) {
		std::string randomSentence = dict.generateSentence(8); // generate sentence with 8 words
		std::cout << "Sentece " << i+1 << ": " << randomSentence << std::endl;
	}


	std::cout << "Press Enter to continue...";
	std::cin.get();

	return 0;
}
