#include "../include/dictionary.hpp"

#include <iostream>


// initialize seed words.. 

Dictionary::Dictionary() {
	words.push_back("the");
	words.push_back("quick");
	words.push_back("brown");
	words.push_back("fox");
	words.push_back("jumps");
	words.push_back("over");
	words.push_back("lazy");
	words.push_back("dog");


	std::cout << "Created dictionary with " << words.size() << " seed words." << std::endl;
}

void Dictionary::addWord(const std::string& word) {
	words.push_back(word);
}

int Dictionary::getWordCount() const {
	return words.size();
}

void Dictionary::showWords() const {
	std::cout << "Dictionary contains " << words.size() << " words:" << std::endl;
	for (const std::string& word : words) {
		std::cout << "- " << word << std::endl;
	}
	std::cout << std::endl;
}

