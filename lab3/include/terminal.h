#ifndef TERMINAL_H
#define TERMINAL_H

#include <buddy.h>

class Terminal;

struct NameFuncMap {
    const char* command = nullptr;
    void(Terminal::*func)() = nullptr;
};

class Terminal {
public:
    Terminal();
    void Run();
private:
    void Ls();
    void Help();
    void Hello();
    void Reboot();
    void Malloc();
    void Free();
    char buffer[1024];
    Buddy<10> buddy;
    NameFuncMap map[10] = {
        {"ls", &Terminal::Ls},
        {"help", &Terminal::Help},
        {"hello", &Terminal::Hello},
        {"reboot", &Terminal::Reboot},
        {"malloc", &Terminal::Malloc},
        {"free", &Terminal::Free},
    };
};

#endif
