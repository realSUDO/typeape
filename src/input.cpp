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
