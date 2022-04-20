#pragma once

#include <Windows.h> // for COORD, SetConsoleCursorPos, GetConsoleCursorInfo, SetConsoleCursorInfo

class Renderer
{
private:
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

protected:
	void gotoXY(int x, int y); // Sets console cursor position
	void setConsoleColour(int colour); // Sets console text colour
	void clearScreen(); // Clears screen
	void hideCursor(); // hides cursor, only needs to be called once
	void resetCursor(); // Sets cursor position back to 0/0 and colour back to white

};

