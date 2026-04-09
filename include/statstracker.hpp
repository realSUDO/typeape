#pragma once
#include <string>
#include <vector>

struct Stats {
  double wpm;
  double rawWpm;
  double accuracy;
  int correctChars;
  int incorrectChars;
  double consistency;
};

class StatsTracker {
public:
  // call on every space (word commit)
  void commitWord(const std::string &target, const std::string &typed, double elapsedSec);

  // call on every keypress (for accuracy)
  void recordKeypress(bool correct);

  Stats compute(int totalSeconds) const;

private:
  struct WordEntry {
    std::string target;
    std::string typed;
    double elapsedSec;
  };

  std::vector<WordEntry> entries;
  int totalKeypresses  = 0;
  int correctKeypresses = 0;
};
