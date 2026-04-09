#pragma once

#include <string>

void clearScreen();
void setGrayColor();
void setWhiteColor();
void resetColor();
void printColoredText(const std::string& text,bool isTyped = false);

