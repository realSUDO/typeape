#include "../include/game.hpp"
#include "../include/dictionary.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"
#include "../include/rowbuffer.hpp"
#include "../include/statstracker.hpp"
#include <chrono>
#include <csignal>
#include <iostream>

static volatile sig_atomic_t g_resized = 0;
static void onResize(int) { g_resized = 1; }

static int chooseTimer() {
  std::cout << "Select timer:\n"
            << "  1) 15s\n  2) 30s (default)\n  3) 45s\n  4) 60s\n  5) Custom\n> ";
  int choice;
  std::cin >> choice;
  switch (choice) {
    case 1: return 15;
    case 3: return 45;
    case 4: return 60;
    case 5: { int s; std::cout << "Seconds: "; std::cin >> s; return s; }
    default: return 30;
  }
}

Game::Game() {}

void Game::run() {
  signal(SIGWINCH, onResize);

  int totalSeconds = chooseTimer();

  Dictionary dict;
  dict.setFilePath("data/words.txt");

  KeyboardInput input;
  bool quit = false;

  while (!quit) {
    // --- race setup ---
    RowBuffer buf(dict);
    StatsTracker stats;
    std::string currentTyping;
    bool running  = true;
    bool started  = false;
    bool restart  = false;
    int  secondsLeft = totalSeconds;
    auto startTime   = std::chrono::steady_clock::now();

    std::cout << "\033[5 q";

    while (running) {
      if (g_resized) { g_resized = 0; }

      if (started) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startTime).count();
        secondsLeft = totalSeconds - (int)elapsed;
        if (secondsLeft <= 0) break;
      }

      auto view = buf.getView();
      clearScreen();
      displayRows(view.rows, view.typedRows, view.cursorRow, view.cursorWord,
                  currentTyping, secondsLeft);

      char key = input.getKey();

      // enter = quick restart (mid-race or after)
      if (key == '\r' || key == '\n') { restart = true; break; }

      if (!started && key != 27) {
        started   = true;
        startTime = std::chrono::steady_clock::now();
        std::cout << "\033[6 q";
      }

      if (started) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startTime).count();
        if (totalSeconds - (int)elapsed <= 0) break;
      }

      if (input.isEscape(key)) {
        running = false; quit = true;
      } else if (input.isBackspace(key)) {
        if (!currentTyping.empty()) {
          currentTyping.pop_back();
          stats.recordKeypress(false);
        }
      } else if (key == ' ') {
        if (!currentTyping.empty()) {
          std::string target = buf.currentTarget();
          double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::steady_clock::now() - startTime).count() / 1000.0;
          stats.commitWord(target, currentTyping, elapsed);
          buf.commitWord(currentTyping);
          currentTyping.clear();
        }
      } else if (key >= 32 && key <= 126) {
        std::string target = buf.currentTarget();
        if (currentTyping.size() < target.size() + 10) {
          bool correct = currentTyping.size() < target.size() &&
                         target[currentTyping.size()] == key;
          stats.recordKeypress(correct);
          currentTyping += key;
        }
      }
    }

    if (restart) continue;
    if (quit) break;

    // --- show stats, wait for enter/esc/q ---
    std::cout << "\n\033[0 q";
    displayStats(stats.compute(totalSeconds));
    std::cout << "\n[enter] restart  [esc/q] quit\n";

    while (true) {
      char k = input.getKey();
      if (k == '\r' || k == '\n') { break; }          // restart
      if (k == 27 || k == 'q')   { quit = true; break; } // exit
    }
  }
}
