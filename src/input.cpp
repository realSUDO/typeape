#include "../include/input.hpp"
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
