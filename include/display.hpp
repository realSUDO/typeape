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
#include "wordtracker.hpp"
#include "statstracker.hpp"

void clearScreen();
void displayHeader();
void displayHeaderFooter();
void setGrayColor();
void setWhiteColor();
void setRedColor();
void setGreenColor();
void setYellowColor();
void resetColor();

// display 3 rows; cursorRow=0 or 1, cursorWord=word index within that row
void displayRows(const std::vector<std::vector<std::string>> &rows,
                 const std::vector<std::vector<std::string>> &typedRows,
                 int cursorRow, int cursorWord,
                 const std::string &currentTyping,
                 int secondsLeft);

void displayStats(const Stats &stats);
