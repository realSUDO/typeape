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
