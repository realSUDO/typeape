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
