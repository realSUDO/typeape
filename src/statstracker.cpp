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

  int correctChars = 0, incorrectChars = 0;
  int allTypedChars = 0; // every char user actually typed (for raw wpm)
  int correctWordChars = 0; // chars in correctly typed words (for wpm)
  int correctWords = 0;

  for (size_t i = 0; i < entries.size(); i++) {
    const auto &e = entries[i];
    bool wordCorrect = (e.target == e.typed);

    // +1 for the space after each word (except conceptually it's included in raw)
    int typedLen = (int)e.typed.size() + 1; // +1 for space
    allTypedChars += typedLen;

    if (wordCorrect) {
      correctWords++;
      correctWordChars += typedLen;
      correctChars += (int)e.target.size();
    } else {
      // char-level diff for the chars stat
      int minLen = std::min(e.target.size(), e.typed.size());
      for (int j = 0; j < minLen; j++) {
        if (e.target[j] == e.typed[j]) correctChars++;
        else incorrectChars++;
      }
      incorrectChars += std::abs((int)e.target.size() - (int)e.typed.size());
    }
  }

  double minutes = totalSeconds / 60.0;
  s.wpm    = (correctWordChars / 5.0) / minutes;
  s.rawWpm = (allTypedChars    / 5.0) / minutes;
  s.accuracy = totalKeypresses > 0 ? (correctKeypresses * 100.0 / totalKeypresses) : 0;
  s.correctChars   = correctChars;
  s.incorrectChars = incorrectChars;

  // consistency: CV of per-word-interval raw wpm, mapped to 0-100
  std::vector<double> intervalWpm;
  double prev = 0.0;
  for (const auto &e : entries) {
    double interval = e.elapsedSec - prev;
    if (interval > 0) {
      double wpm = (((int)e.typed.size() + 1) / 5.0) / (interval / 60.0);
      intervalWpm.push_back(wpm);
    }
    prev = e.elapsedSec;
  }

  if (intervalWpm.size() > 1) {
    double mean = std::accumulate(intervalWpm.begin(), intervalWpm.end(), 0.0) / intervalWpm.size();
    if (mean > 0) {
      double variance = 0;
      for (double v : intervalWpm) variance += (v - mean) * (v - mean);
      variance /= intervalWpm.size();
      double cv = std::sqrt(variance) / mean;
      s.consistency = std::max(0.0, 100.0 - cv * 100.0);
    }
  } else {
    s.consistency = 100.0;
  }

  return s;
}
