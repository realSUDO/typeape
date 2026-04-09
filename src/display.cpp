#include "../include/display.hpp"

#include <cstddef>
#include <iostream>
#include <string>

// terminal colors..
const std::string GRAY_COLOR = "\033[90m";
const std::string WHITE_COLOR = "\033[97m";
const std::string RESET_COLOR = "\033[0m";
const std::string RED_COLOR = "\033[31m";
const std::string GREEN_COLOR = "\033[32m";
const std::string YELLOW_COLOR = "\033[33m";

void clearScreen() {
#ifdef _WIN32
  system("cls"); // windows.. ew
#else
  system("clear"); // unix
#endif
}

void setGrayColor() { std::cout << GRAY_COLOR; }

void setWhiteColor() { std::cout << WHITE_COLOR; }

void setRedColor() { std::cout << RED_COLOR; }

void setGreenColor() { std::cout << GREEN_COLOR; }

void setYellowColor() { std::cout << YELLOW_COLOR; }

void resetColor() { std::cout << RESET_COLOR; }

void printColoredText(const std::string &text, bool isTyped) {
  if (isTyped) {
    setWhiteColor();
  } else {
    setGrayColor();
  }
  std::cout << text;
  resetColor();
}

void displayWordBasedText(const std::vector<std::string> &targetWords,
                          const std::vector<std::string> &typeWords,
                          int currentWordIndex,
                          const std::string &currentTyping) {

  int cursorCol = 1; // 1-based column for ANSI
  int col = 1;

  for (size_t i = 0; i < targetWords.size(); i++) {
    if (i < currentWordIndex) {
      std::string typed = typeWords[i];
      std::string target = targetWords[i];

      bool wordPerfect = typed.length() == target.length();
      for (size_t j = 0; wordPerfect && j < typed.length(); j++)
        if (typed[j] != target[j]) wordPerfect = false;

      wordPerfect ? setGreenColor() : setYellowColor();
      std::cout << targetWords[i];
      resetColor();
    } else if (i == currentWordIndex) {
      std::string target = targetWords[i];

      // cursor sits after the typed portion (including overflow)
      cursorCol = col + (int)currentTyping.length();

      for (size_t j = 0; j < target.length(); j++) {
        if (j < currentTyping.length()) {
          currentTyping[j] == target[j] ? setGreenColor() : setRedColor();
        } else {
          setGrayColor();
        }
        std::cout << target[j];
        resetColor();
      }
      // overflow chars beyond target length
      if (currentTyping.length() > target.length()) {
        setRedColor();
        std::cout << currentTyping.substr(target.length());
        resetColor();
      }
    } else {
      setGrayColor();
      std::cout << targetWords[i];
    }

    resetColor();

    if (i < targetWords.size() - 1) {
      std::cout << " ";
      col += (int)targetWords[i].length() + 1;
    }
  }

  std::cout << std::endl;

  // move cursor back up and to the correct column
  std::cout << "\033[A\033[" << cursorCol << "G";
  std::cout.flush();
}

void displayWordAccuracy(const std::vector<WordResult> &results) {
  std::cout << "\n--- Word-by-Word Accuracy ---\n";

  double totalAccuracy = 0;
  int perfectWords = 0;

  for (const auto &result : results) {
    std::cout << result.targetWord << " : " << result.typedWord << " → ";

    if (result.accuracy == 100.0) {
      setGreenColor();
      std::cout << "✓ " << (int)result.accuracy << "%";
      perfectWords++;
    } else if (result.accuracy >= 70.0) {
      setYellowColor();
      std::cout << (int)result.accuracy << "%";
    } else {
      setRedColor();
      std::cout << (int)result.accuracy << "%";
    }

    resetColor();
    std::cout << " (" << result.correctChars << "/" << result.totalChars
              << " chars)" << std::endl;

    totalAccuracy += result.accuracy;
  }

  double avgAccuracy = totalAccuracy / results.size();
  std::cout << "\nOverall Word Accuracy: ";
  if (avgAccuracy >= 90)
    setGreenColor();
  else if (avgAccuracy >= 70)
    setYellowColor();
  else
    setRedColor();
  std::cout << (int)avgAccuracy << "%";
  resetColor();

  std::cout << " | Perfect words: " << perfectWords << "/" << results.size()
            << std::endl;
}
