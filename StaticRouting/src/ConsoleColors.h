#pragma once


#include <Windows.h>
class Console
{
public:
    static HANDLE getConsoleHandle() {
        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        return hConsole;
    }
};

void set_console_error()
{
    SetConsoleTextAttribute(Console::getConsoleHandle(), FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void reset_console_color()
{
    SetConsoleTextAttribute(Console::getConsoleHandle(), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void set_console_warning()
{
    SetConsoleTextAttribute(Console::getConsoleHandle(), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void set_console_success()
{

    SetConsoleTextAttribute(Console::getConsoleHandle(), FOREGROUND_GREEN);
}

void set_console_info()
{
    SetConsoleTextAttribute(Console::getConsoleHandle(), FOREGROUND_GREEN | FOREGROUND_BLUE);
}
