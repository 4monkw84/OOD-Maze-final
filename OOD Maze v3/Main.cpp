#include "GameController.h"

#include <string>

int main()
{
	std::wstring windowTitle = L"2D Maze Game";
	SetConsoleTitle(windowTitle.c_str());

	GameController v;
	v.printMenu();
	v.getInput();
}