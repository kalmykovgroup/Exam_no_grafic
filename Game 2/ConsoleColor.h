#pragma once
#include "Header.h"

enum  Color
{
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};


class ConsoleColor
{

public:
    void getTest() {

    }
    class SetTextColor
    {
    private:
        static void perform(Color color) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hStdOut, color);
        }
    public:

        static void Black() { perform(Color::Black); }
        static void Blue() { perform(Color::Blue); }
        static void Green() { perform(Color::Green); }
        static void Cyan() { perform(Color::Cyan); }
        static void Red() { perform(Color::Red); }
        static void Magenta() { perform(Color::Magenta); }
        static void Brown() { perform(Color::Brown); }
        static void LightGray() { perform(Color::LightGray); }
        static void LightBlue() { perform(Color::LightBlue); }
        static void LightGreen() { perform(Color::LightGreen); }
        static void LightCyan() { perform(Color::LightCyan); }
        static void LightRed() { perform(Color::LightRed); }
        static void LightMagenta() { perform(Color::LightMagenta); }
        static void Yellow() { perform(Color::Yellow); }
        static void White() { perform(Color::White); }

    };



};

