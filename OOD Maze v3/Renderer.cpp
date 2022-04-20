#include "Renderer.h"

#include <iostream>

void Renderer::gotoXY(int x, int y)
{
	COORD pos{ static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(hOut, pos);
}

void Renderer::setConsoleColour(int colour)
{
	SetConsoleTextAttribute(hOut, colour);
}

void Renderer::clearScreen()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOut, &info);
	DWORD d = info.srWindow.Bottom * info.srWindow.Right;
	COORD position = { 0,0 };
	FillConsoleOutputCharacter(hOut, ' ', d, position, &d);
}

void Renderer::hideCursor()
{
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hOut, &cursorInfo);
}

void Renderer::resetCursor()
{
	gotoXY(0, 0);
	setConsoleColour(15);
}
