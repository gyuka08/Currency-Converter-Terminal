#pragma once

#include <iostream>
#include <Windows.h>
#include <functional>

void border(int len) {              //��
    for (int i = 0; i < len; i++) {
        std::cout << "=" << std::ends;
    }
    std::cout << std::endl;
}

void reTerm(char fill = ' ') {            //������ ASSEMBLY �ڵ�(�͹̳� �缳��)
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}