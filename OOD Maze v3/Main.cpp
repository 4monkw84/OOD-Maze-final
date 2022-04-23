#include "GameController.h"

#include <string>

int main()
{
	std::wstring windowTitle = L"2D Maze Game - Will Monk";
	SetConsoleTitle(windowTitle.c_str());

	GameController v;
	v.printMenu();
	v.getInput();
}