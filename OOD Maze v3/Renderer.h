#pragma once

#include <Windows.h> // for COORD, SetConsoleCursorPos, GetConsoleCursorInfo, SetConsoleCursorInfo

class Renderer
{
private:
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

protected:
	void gotoXY(int x, int y);
	void setConsoleColour(int colour);
	void clearScreen();
	void hideCursor();
	void resetCursor();
};

