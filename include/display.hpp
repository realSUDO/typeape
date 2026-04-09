#pragma once

#include <string>
#include <vector>
#include "wordtracker.hpp"
#include "statstracker.hpp"

void clearScreen();
void displayHeader();
void displayHeaderFooter();
void setGrayColor();
void setWhiteColor();
void setRedColor();
void setGreenColor();
void setYellowColor();
void resetColor();

// display 3 rows; cursorRow=0 or 1, cursorWord=word index within that row
void displayRows(const std::vector<std::vector<std::string>> &rows,
                 const std::vector<std::vector<std::string>> &typedRows,
                 int cursorRow, int cursorWord,
                 const std::string &currentTyping,
                 int secondsLeft);

void displayStats(const Stats &stats);
