/*
 * TypeApe - Terminal Typing Game
 * Copyright (C) 2026 realSUDO <github.com/realSUDO>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "../include/wordtracker.hpp"
#include <algorithm> // for std::min
#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream> // for std::istringstream
#include <string>
#include <vector>

WordTracker::WordTracker() {
  currentWordIndex = 0;
  wordComplete = false;
}

void WordTracker::setTargetText(const std::string &sentence) {
  targetWords.clear();
  typedWords.clear();

  currentWordIndex = 0;
  currentTyping = "";

  std::stringstream ss(sentence);
  std::string word;

  while (ss >> word) {
    targetWords.push_back(word);
  }

  // Initialize typed words vector..

  typedWords.resize(targetWords.size(), ""); // start with empty typed words..
}

void WordTracker::handleChar(char c) {
  if (wordComplete)
    return;
  std::string target = currentWordIndex < targetWords.size() ? targetWords[currentWordIndex] : "";
  if (currentTyping.length() >= target.length() + 10)
    return;
  currentTyping += c;
}

void WordTracker::handleBackspace() {
  if (wordComplete)
    return;

  if (!currentTyping.empty()) {
    currentTyping.pop_back();
  }
}

void WordTracker::handleSpace() {
  if (wordComplete)
    return;

  // save current typed word...

  if (currentWordIndex < targetWords.size()) {
    typedWords[currentWordIndex] = currentTyping;
    currentWordIndex++;
    currentTyping = ""; // reset for next word..

    // check if sentence complete..
    if (currentWordIndex >= targetWords.size()) {
      wordComplete = true;
    }
  }
}

double WordTracker::calculateWordAccuracy(const std::string& target , const std::string& typed) {
	if (target.empty()) return 0.0;
	if(typed.empty()) return 0.0;

	int correct = 0 ; 
	int maxLength = std::min(target.length(), typed.length());

	// compare char by char.. 

	for (int i = 0 ; i < maxLength ; i++) {
		if (target[i] == typed[i] ) {
			correct++;
		}
	}

	return  (correct * 100.0) / target.length(); // accuracy percentage based on target length..
}


std::vector<WordResult> WordTracker::getResults() {
	std::vector<WordResult> results;

	for (size_t i = 0 ; i < targetWords.size(); i++ ) {
		WordResult result;
		result.targetWord = targetWords[i];
		result.typedWord = typedWords[i];

		result.accuracy = calculateWordAccuracy(targetWords[i],typedWords[i]);

		// calculate correct chars.. 

		result.correctChars = 0 ; 
		int maxLength = std::min(targetWords[i].length(), typedWords[i].length());

		for (int j = 0 ; j < maxLength ; j++ ) {
			if (targetWords[i][j] == typedWords[i][j]) {
				result.correctChars++;
			}
		}

		result.totalChars = targetWords[i].length();
		result.isCorrect = (result.accuracy == 100.0);

		results.push_back(result);
		
	}
	return results;
}

double WordTracker:: getOverallAccuracy() {
	if (targetWords.empty()) return 0.0;

	double totalAccuracy = 0.0;
	for (size_t i = 0 ; i < targetWords.size() ; i++ ) {
		totalAccuracy += calculateWordAccuracy(targetWords[i], typedWords[i]);
	}
	return  totalAccuracy / targetWords.size(); // average accuracy across all words..
}


int WordTracker::getTotalCorrectChars() {
	int total = 0 ; 
	for (size_t i = 0 ; i < targetWords.size(); i++) {
		int maxLength = std::min(targetWords[i].length(), typedWords[i].length());
		for (int j = 0 ; j < maxLength ; j++ ) {
			if (targetWords[i][j] == typedWords[i][j]) {
				total++;
			}
		}
	}
	return  total;
}


int WordTracker::getTotalChars() {
	int total = 0 ; 
	for (const auto& word : targetWords) {
		total += word.length();
	}
	return total;
}
