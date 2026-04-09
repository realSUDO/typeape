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
#include "../include/dictionary.hpp"
#include <chrono>
#include <fstream>
#include <string>

Dictionary::Dictionary()
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

std::string Dictionary::generateSentence(int wordCount) const {
  std::ifstream file(filePath, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    // fallback
    return "the quick brown fox jumps over the lazy dog";
  }

  std::streamsize fileSize = file.tellg();

  std::uniform_int_distribution<std::streamsize> dist(0, fileSize - 1);

  std::string sentence;
  int collected = 0;

  while (collected < wordCount) {
    // jump to a random byte
    file.seekg(dist(rng));

    // skip partial word at seek position
    std::string skip;
    file >> skip;

    std::string word;
    if (file >> word) {
      sentence += word;
      if (collected < wordCount - 1) sentence += " ";
      collected++;
    }
  }

  return sentence;
}
