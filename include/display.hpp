#pragma once

#include <string>
#include <vector>
#include "wordtracker.hpp"

void clearScreen();
void setGrayColor();
void setWhiteColor();
void setRedColor();
void setGreenColor();
void setYellowColor();
void resetColor();


void displayWordBasedText(const std::vector<std::string>& targetWords, const std::vector<std::string>& typedWords, int currentWordIndex , const std::string& currentTyping);

void displayWordAccuracy (const std::vector<WordResult>& results);
void printColoredText(const std::string& text,bool isTyped = false);

