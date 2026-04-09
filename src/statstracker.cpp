#include "../include/statstracker.hpp"
#include <cmath>
#include <numeric>

void StatsTracker::commitWord(const std::string &target, const std::string &typed, double elapsedSec) {
  entries.push_back({target, typed, elapsedSec});
}

void StatsTracker::recordKeypress(bool correct) {
  totalKeypresses++;
  if (correct) correctKeypresses++;
}

Stats StatsTracker::compute(int totalSeconds) const {
  Stats s{};

  int correctWords = 0, incorrectWords = 0;
  int correctChars = 0, incorrectChars = 0;

  for (const auto &e : entries) {
    bool wordCorrect = (e.target == e.typed);
    if (wordCorrect) {
      correctWords++;
      correctChars += (int)e.target.size();
    } else {
      incorrectWords++;
      // char-level diff
      int minLen = std::min(e.target.size(), e.typed.size());
      for (int i = 0; i < minLen; i++) {
        if (e.target[i] == e.typed[i]) correctChars++;
        else incorrectChars++;
      }
      incorrectChars += std::abs((int)e.target.size() - (int)e.typed.size());
    }
  }

  // spaces between correct words count as correct chars
  int correctWithSpaces = correctChars + std::max(0, correctWords - 1);

  s.wpm    = (correctWithSpaces / 5.0) / (totalSeconds / 60.0);
  s.rawWpm = ((correctChars + incorrectChars + (int)entries.size() - 1) / 5.0) / (totalSeconds / 60.0);
  s.accuracy = totalKeypresses > 0 ? (correctKeypresses * 100.0 / totalKeypresses) : 0;
  s.correctChars   = correctChars;
  s.incorrectChars = incorrectChars;

  // consistency: coefficient of variation of per-second raw wpm, mapped to 0-100
  // bucket words into 1-second intervals
  std::vector<double> perSecRaw(totalSeconds, 0.0);
  for (const auto &e : entries) {
    int sec = std::min((int)e.elapsedSec, totalSeconds - 1);
    int chars = (int)e.target.size(); // raw = all typed words
    perSecRaw[sec] += chars / 5.0 * 60.0; // wpm contribution
  }

  double mean = std::accumulate(perSecRaw.begin(), perSecRaw.end(), 0.0) / totalSeconds;
  if (mean > 0) {
    double variance = 0;
    for (double v : perSecRaw) variance += (v - mean) * (v - mean);
    variance /= totalSeconds;
    double cv = std::sqrt(variance) / mean; // coefficient of variation
    s.consistency = std::max(0.0, 100.0 - cv * 100.0);
  } else {
    s.consistency = 0;
  }

  return s;
}
