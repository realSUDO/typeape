#include "../include/display.hpp"
#include "../include/statstracker.hpp"
#include <iostream>
#include <string>

const std::string GRAY    = "\033[90m";
const std::string WHITE   = "\033[97m";
const std::string RESET   = "\033[0m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";

void clearScreen() { std::cout << "\033[2J\033[H"; }
void setGrayColor()   { std::cout << GRAY; }
void setWhiteColor()  { std::cout << WHITE; }
void setRedColor()    { std::cout << RED; }
void setGreenColor()  { std::cout << GREEN; }
void setYellowColor() { std::cout << YELLOW; }
void resetColor()     { std::cout << RESET; }

static void renderRow(const std::vector<std::string> &target,
                      const std::vector<std::string> &typed,
                      int activeWord,               // -1 = not active row
                      const std::string &currentTyping,
                      int &cursorCol, int col) {
  for (size_t i = 0; i < target.size(); i++) {
    if ((int)i < activeWord) {
      // completed word
      const std::string &t = typed[i];
      const std::string &tgt = target[i];
      bool perfect = (t == tgt);
      perfect ? std::cout << GREEN : std::cout << YELLOW;
      std::cout << tgt << RESET;
    } else if ((int)i == activeWord) {
      // current word being typed
      cursorCol = col + (int)currentTyping.length();
      for (size_t j = 0; j < target[i].size(); j++) {
        if (j < currentTyping.size())
          std::cout << (currentTyping[j] == target[i][j] ? GREEN : RED);
        else
          std::cout << GRAY;
        std::cout << target[i][j] << RESET;
      }
      // overflow
      if (currentTyping.size() > target[i].size()) {
        std::cout << RED << currentTyping.substr(target[i].size()) << RESET;
      }
    } else {
      std::cout << GRAY << target[i] << RESET;
    }
    if (i + 1 < target.size()) {
      std::cout << " ";
      col += (int)target[i].size() + 1;
    }
  }
  std::cout << "\n";
}

void displayRows(const std::vector<std::vector<std::string>> &rows,
                 const std::vector<std::vector<std::string>> &typedRows,
                 int cursorRow, int cursorWord,
                 const std::string &currentTyping,
                 int secondsLeft) {

  // timer line
  std::cout << WHITE << secondsLeft << "s" << RESET << "\n\n";

  int cursorCol = 1;
  int cursorLine = 3 + cursorRow; // 1 timer + 1 blank + rows

  for (int r = 0; r < 3; r++) {
    int col = 1;
    int activeWord = (r == cursorRow) ? cursorWord : -1;
    renderRow(rows[r], typedRows[r], activeWord, currentTyping, cursorCol, col);
  }

  // move cursor to correct position
  std::cout << "\033[" << cursorLine << ";" << cursorCol << "H";
  std::cout.flush();
}

void displayStats(const Stats &stats) {
  std::cout << "\n"
            << GREEN  << "wpm      " << RESET << (int)stats.wpm        << "\n"
            << GRAY   << "raw      " << RESET << (int)stats.rawWpm     << "\n"
            << GREEN  << "acc      " << RESET << (int)stats.accuracy   << "%\n"
            << GRAY   << "chars    " << RESET
            << GREEN  << stats.correctChars   << RESET << "/"
            << RED    << stats.incorrectChars << RESET << "\n"
            << GRAY   << "consist  " << RESET << (int)stats.consistency << "%\n";
}
