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
#include "../include/input.hpp"
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig;

KeyboardInput::KeyboardInput() {
    tcgetattr(STDIN_FILENO, &orig);
    struct termios raw = orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

KeyboardInput::~KeyboardInput() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig);
}

char KeyboardInput::getKey() {
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

char KeyboardInput::getKeyTimeout(int ms) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    struct timeval tv { ms / 1000, (ms % 1000) * 1000 };
    if (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
        char c;
        read(STDIN_FILENO, &c, 1);
        return c;
    }
    return 0; // timeout
}
