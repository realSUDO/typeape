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
#include "../include/game.hpp"
#include "../include/dictionary.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"
#include "../include/rowbuffer.hpp"
#include "../include/statstracker.hpp"
#include <algorithm>
#include <chrono>
#include <csignal>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

static volatile sig_atomic_t g_resized = 0;
static void onResize(int) { g_resized = 1; }

static int chooseTimer(KeyboardInput &input) {
  const int options[] = {15, 30, 45, 60, 0};
  const char *labels[] = {"15s", "30s", "45s", "60s", "custom"};
  const int count = 5;
  int selected = 1;

  while (true) {
    clearScreen();
    displayHeader();
    std::cout << "\n  select timer\n\n";
    for (int i = 0; i < count; i++) {
      if (i == selected)
        std::cout << "  \033[97m> " << labels[i] << "\033[0m\n";
      else
        std::cout << "  \033[90m  " << labels[i] << "\033[0m\n";
    }
    std::cout << "\n  [↑↓] navigate  [enter] confirm\n";
    displayHeaderFooter();

    char key = input.getKey();
    if (key == '\033') {
      char seq[2];
      read(STDIN_FILENO, &seq[0], 1);
      read(STDIN_FILENO, &seq[1], 1);
      if (seq[0] == '[') {
        if (seq[1] == 'A') selected = (selected - 1 + count) % count;
        if (seq[1] == 'B') selected = (selected + 1) % count;
      }
    } else if (key == '\r' || key == '\n') {
      if (options[selected] == 0) {
        // draw inline input box
        std::string buf;
        while (true) {
          // redraw screen with box
          clearScreen();
          displayHeader();
          std::cout << "\n  select timer\n\n";
          for (int i = 0; i < count; i++) {
            if (i == selected)
              std::cout << "  \033[97m> " << labels[i] << "\033[0m\n";
            else
              std::cout << "  \033[90m  " << labels[i] << "\033[0m\n";
          }
          // box
          std::cout << "\n";
          std::cout << "  ┌─────────────────────┐\n";
          std::cout << "  │ seconds: " << buf << "\033[97m_\033[0m";
          int pad = 11 - (int)buf.size();
          std::cout << std::string(std::max(0,pad), ' ') << "│\n";
          std::cout << "  └─────────────────────┘\n";
          std::cout.flush();

          char k = input.getKey();
          if (k == '\r' || k == '\n') {
            if (!buf.empty()) return std::stoi(buf);
          } else if (k == 27) {
            break; // cancel
          } else if ((k == 127 || k == 8) && !buf.empty()) {
            buf.pop_back();
          } else if (k >= '0' && k <= '9' && buf.size() < 5) {
            buf += k;
          }
        }
        selected = 1; // cancelled, reset to default
        continue;
      }
      return options[selected];
    }
  }
}

static void goodbye() {
  clearScreen();
  std::cout << "\n\n\nHave a nice day.. with your lazy fingers\n\n\n";
}

Game::Game() {}

void Game::run() {
  signal(SIGWINCH, onResize);

  Dictionary dict;
  dict.setFilePath(TYPEAPE_DATA_DIR "/words.txt");

  KeyboardInput input;
  bool quit = false;

  int totalSeconds = chooseTimer(input);

  while (!quit) {
    RowBuffer buf(dict);
    StatsTracker stats;
    std::string currentTyping;
    bool freshWord   = false; // true = on new word, no chars typed yet → backspace allowed
    bool running     = true;
    bool started     = false;
    bool restart     = false;
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
      displayHeader();
      displayRows(view.rows, view.typedRows, view.cursorRow, view.cursorWord,
                  currentTyping, secondsLeft);

      char key = started ? input.getKeyTimeout(1000) : input.getKey();
      if (key == 0) continue; // timeout — just redraw with updated timer

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
        quit = true; running = false;
      } else if (input.isBackspace(key)) {
        if (!currentTyping.empty()) {
          currentTyping.pop_back();
          stats.recordKeypress(false);
        } else if (freshWord) {
          // restore previous word
          buf.uncommitWord(currentTyping);
          freshWord = false;
        }
      } else if (key == ' ') {
        if (!currentTyping.empty()) {
          std::string target = buf.currentTarget();
          double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::steady_clock::now() - startTime).count() / 1000.0;
          stats.commitWord(target, currentTyping, elapsed);
          buf.commitWord(currentTyping);
          currentTyping.clear();
          freshWord = true;
        }
      } else if (key >= 32 && key <= 126) {
        freshWord = false; // locked in, can't go back
        std::string target = buf.currentTarget();
        if (currentTyping.size() < target.size() + 10) {
          bool correct = currentTyping.size() < target.size() &&
                         target[currentTyping.size()] == key;
          stats.recordKeypress(correct);
          currentTyping += key;
        }
      }
    }

    if (quit) { goodbye(); break; }
    if (restart) continue;

    std::cout << "\n\033[0 q";
    displayStats(stats.compute(totalSeconds));
    std::cout << "\n  [enter] restart  [esc/q] quit\n";

    while (true) {
      char k = input.getKey();
      if (k == '\r' || k == '\n') { break; }
      if (k == 27 || k == 'q')   { quit = true; break; }
    }
    if (quit) { goodbye(); break; }
  }
}
