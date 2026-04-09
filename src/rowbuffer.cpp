#include "../include/rowbuffer.hpp"
#include <string>

RowBuffer::RowBuffer(Dictionary &dict) : dict(dict) {
  // seed initial words synchronously
  for (int i = 0; i < PREFETCH_AHEAD; i++) {
    words.push_back(dict.generateSentence(1));
    typed.push_back("");
  }
  worker = std::thread(&RowBuffer::prefetchLoop, this);
}

RowBuffer::~RowBuffer() {
  stop.store(true);
  cv.notify_all();
  if (worker.joinable()) worker.join();
}

void RowBuffer::prefetchLoop() {
  while (!stop.load()) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] {
      return stop.load() || (int)words.size() - cursorAbs < PREFETCH_AHEAD;
    });
    if (stop.load()) break;
    lock.unlock();

    std::string w = dict.generateSentence(1);

    lock.lock();
    words.push_back(std::move(w));
    typed.push_back("");
  }
}

RowBuffer::View RowBuffer::getView() const {
  std::lock_guard<std::mutex> lock(mtx);
  int wpr = calcWordsPerRow();

  View v;
  v.rows.resize(3);
  v.typedRows.resize(3);

  for (int r = 0; r < 3; r++) {
    for (int w = 0; w < wpr; w++) {
      int idx = windowStart + r * wpr + w;
      if (idx < (int)words.size()) {
        v.rows[r].push_back(words[idx]);
        v.typedRows[r].push_back(typed[idx]);
      }
    }
  }

  int rel = cursorAbs - windowStart;
  v.cursorRow  = rel / wpr;
  v.cursorWord = rel % wpr;

  return v;
}

bool RowBuffer::commitWord(const std::string &t) {
  std::lock_guard<std::mutex> lock(mtx);
  typed[cursorAbs] = t;
  cursorAbs++;

  int wpr = calcWordsPerRow();
  // cursor is now on row 2 (0-indexed) relative to window — scroll
  int rel = cursorAbs - windowStart;
  bool scrolled = false;
  if (rel >= wpr * 2) {
    windowStart += wpr; // drop row 0, shift up
    scrolled = true;
  }

  cv.notify_all();
  return scrolled;
}

std::string RowBuffer::currentTarget() const {
  std::lock_guard<std::mutex> lock(mtx);
  if (cursorAbs < (int)words.size()) return words[cursorAbs];
  return "";
}
