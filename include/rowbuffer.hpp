#pragma once
#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include "../include/dictionary.hpp"

static inline int calcWordsPerRow() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int cols = (w.ws_col > 0) ? w.ws_col : 80;
  int words = cols / 7;
  if (words < 5)  words = 5;
  if (words > 15) words = 15;
  return words;
}

class RowBuffer {
public:
  RowBuffer(Dictionary &dict);
  ~RowBuffer();

  struct View {
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<std::string>> typedRows;
    int cursorRow;
    int cursorWord;
  };

  // re-wrap same words at current terminal width, cursor preserved
  View getView() const;

  // commit typed word at cursor, advance — returns true if scroll happened
  bool commitWord(const std::string &typed);

  // undo last commit (only if no chars typed in current word yet)
  void uncommitWord(std::string &restoredTyping);

  std::string currentTarget() const;

private:
  void prefetchLoop();

  Dictionary &dict;

  std::deque<std::string> words;  // target words (flat)
  std::deque<std::string> typed;  // parallel typed (empty = not typed yet)
  int cursorAbs = 0;              // index of current word in words[]
  int windowStart = 0;            // index of first word on screen (row 0)

  mutable std::mutex mtx;
  std::condition_variable cv;
  std::thread worker;
  std::atomic<bool> stop{false};

  static constexpr int PREFETCH_AHEAD = 60;
};
