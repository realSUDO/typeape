#pragma once

#include <random>
#include <string>

class Dictionary {
public:
  Dictionary();
  void setFilePath(const std::string &path) { filePath = path; }
  std::string generateSentence(int wordCount) const;

private:
  std::string filePath;
  mutable std::mt19937 rng;
};
