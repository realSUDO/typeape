#pragma once

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

};
