#pragma once

class KeyboardInput {
public:
    KeyboardInput();
    ~KeyboardInput();
    char getKey();
    bool isEscape(char c) const { return c == 27; }
    bool isBackspace(char c) const { return c == 127 || c == 8; }
};
