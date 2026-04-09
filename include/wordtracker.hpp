#pragma once
#include <string>
#include <vector>

struct WordResult {
  std::string targetWord;
  std::string typedWord;

  double accuracy; // per word accuracy..
  int correctChars;
  int totalChars;
  bool isCorrect;
};

class WordTracker {
private:
  std::vector<std::string> targetWords; // og sentence words..

  std::vector<std::string> typedWords; // user typed words..

  int currentWordIndex;
  std::string currentTyping;
  bool wordComplete;

public:
  WordTracker();

  void setTargetText(const std::string &sentence);
  void handleChar(char c);
  void handleBackspace();
  void handleSpace();

  double calculateWordAccuracy(const std::string &target,
                               const std::string &typed);

// get current result.. 
	std::vector<WordResult> getResults() ;

	// get current word being typed..
	std::string getCurrentWord() const { return currentTyping; }

	std::string getCurrentTarget() const {
		return currentWordIndex < targetWords.size() ? targetWords[currentWordIndex] : "";
	}

	// check for sentence completion..
	bool isSentenceComplete() const { return currentWordIndex >= targetWords.size(); }


	// get overall stats.. 

	double getOverallAccuracy();
	int getTotalCorrectChars();
	int getTotalChars();

	// get progress..
	int getCompletedWords() const { return currentWordIndex; }
	int getTotalWords() const { return targetWords.size(); }

	const std::vector<std::string>& getTargetWords() const { return targetWords; }
	const std::vector<std::string>& getTypedWords() const { return typedWords; }
};
