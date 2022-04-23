#pragma once

#include "MazeGame.h"
#include "Renderer.h"

#include <iostream> // for cout
#include <vector> // for vector
#include <string> // for string

class GameController : public Renderer
{
	MazeGame game;
	int highscore = 0;

	enum Colours { white = 15, yellow = 14, pink = 13, red = 12, aqua = 11, green = 10 };

	std::vector<std::string> dynamicText;

	//positions for menu text
	const short staticX = 5;
	const short staticY = 20;
	const short dynamicX = staticX;
	const short dynamicY = staticY + 2;

	void menuActions(int input);
	void printStatic();
	void printDynamic();
	void updateElement(int element, int colour);
	void gameOver();
	void startGame();
public:
	GameController();
	void printMenu();
	void getInput();
};