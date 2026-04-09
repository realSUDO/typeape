#pragma once

#include <random>
#include <string>
#include <vector>

class Dictionary {
private:
  std::vector<std::string> words; // dynamic array of stringg..
  public:
	Dictionary();
	void addWord(const std::string& word) ;

	int getWordCount() const ; 

	void showWords() const;

	std::string getRandomWord() const; // to get raddom words.. 

	std::string generateSentence(int wordCount) const;

private : 
	// new radom generator.. 
	mutable std::mt19937 rng; // mutable to allow modification in const method


};
