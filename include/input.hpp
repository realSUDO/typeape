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

class KeyboardInput {
public:
    KeyboardInput();
    ~KeyboardInput();
    char getKey();
    // returns 0 if timeout (ms), otherwise the key
    char getKeyTimeout(int ms);
    bool isEscape(char c) const { return c == 27; }
    bool isBackspace(char c) const { return c == 127 || c == 8; }
};
