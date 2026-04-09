#include "../include/dictionary.hpp"
#include <chrono> // for seeding random generator with time..
#include <cstddef>
#include <iostream>
#include <random> // for random number ..

// initialize seed words..

// updat : added random word generator..
Dictionary::Dictionary()
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
  words.push_back("the");
  words.push_back("quick");
  words.push_back("brown");
  words.push_back("fox");
  words.push_back("jumps");
  words.push_back("over");
  words.push_back("lazy");
  words.push_back("dog");

  words.push_back("headphones");
  words.push_back("keyboard");
  words.push_back("monitor");
  words.push_back("mouse");
  words.push_back("programming");
  words.push_back("typing");
  words.push_back("challenge");

  std::cout << "Created dictionary with " << words.size() << " seed words."
            << std::endl;
}

void Dictionary::addWord(const std::string &word) { words.push_back(word); }

int Dictionary::getWordCount() const { return words.size(); }

void Dictionary::showWords() const {
  std::cout << "Dictionary contains " << words.size() << " words:" << std::endl;
  for (const std::string &word : words) {
    std::cout << "- " << word << std::endl;
  }
  std::cout << std::endl;
}

// get random word from dictionary..
std::string Dictionary::getRandomWord() const {
  if (words.empty())
    return ""; // empty string if no words available

  // create uniform distribution.. from 0 to size 1
  std::uniform_int_distribution<size_t> dist(0, words.size() - 1);

  // random index...
  size_t index = dist(rng);

  return words[index];
}

// generate a sentence..

std::string Dictionary::generateSentence(int wordCount) const {
  std::string sentence;

  for (int i = 0; i < wordCount; i++) {
    std::string word = getRandomWord();

    // capitalize first letter..
    if (i == 0 && !word.empty()) {
      word[0] = toupper(word[0]);
    }
    sentence += word;

    if (i < wordCount - 1) {
      sentence += " "; // add space between words
    }
  }
  sentence += "."; // end with a period..
  return sentence;
}

