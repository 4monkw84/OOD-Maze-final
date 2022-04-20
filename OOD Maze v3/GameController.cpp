#include "GameController.h"

#include <vector> // for vector
#include <conio.h> // for _kbhit(), _getch()

void GameController::menuActions(int input)
{
	switch (input)
	{
	case 0:
		game = new MazeGame;
		game->startGame();
		gameOver();
		break;

	case 1:
		exit(0);
	}
}

void GameController::printStatic()
{
	short targetX = staticX;
	short targetY = staticY;
	setConsoleColour(aqua);
	gotoXY(targetX, targetY);
	printf("2D Maze Game.");
}

void GameController::printDynamic()
{
	short targetX = dynamicX;
	short targetY = dynamicY;
	setConsoleColour(white);
	for (size_t i = 0; i < dynamicText.size(); i++)
	{
		gotoXY(targetX, targetY);
		std::cout << dynamicText.at(i);
		targetY++;
	}
}

void GameController::updateElement(int element, int colour)
{
	gotoXY(dynamicX, dynamicY + element);
	setConsoleColour(colour);
	std::cout << dynamicText.at(element);
}

void GameController::gameOver()
{
	clearScreen();

	short targetX = staticX;
	short targetY = staticY - 10;

	gotoXY(targetX, targetY);

	if (game->getHealth() > 0)
	{
		setConsoleColour(green);
		printf("You win!");
	}
	else
	{
		setConsoleColour(red);
		printf("You lose!");
	}
	targetY++;
	gotoXY(targetX, targetY);
	printf("Your final score was: %i", game->getScore());

	if (highscore == 0)
		highscore = game->getScore();
	if (game->getScore() > highscore)
		highscore = game->getScore();

	targetY++;
	gotoXY(targetX, targetY);
	printf("Your best score was: %i", highscore);

	targetY += 10;
	gotoXY(targetX, targetY);
	printf("Play again? ( Y / N )");

	enum inputs { Y = 121, N = 110 };

	bool inMenu = true;
	while (inMenu)
	{
		switch (_getch())
		{
		case Y:
			delete game;
			game = new MazeGame;
			startGame();
			break;

		case N:
			exit(0);
			break;

		default:
			break;
		}
	}
}

void GameController::startGame()
{
	game = new MazeGame;
	game->startGame();
	gameOver();
}

GameController::GameController()
{
	dynamicText.push_back("Play");
	dynamicText.push_back("Quit");
}

void GameController::printMenu()
{
	printStatic();
	printDynamic();
	updateElement(0, green);
	hideCursor();
	resetCursor();
}

void GameController::getInput()
{
	bool inMenu = true;
	int previousElement = 0;
	int targetElement = 0;
	while (inMenu)
	{
		if (_kbhit())
		{
			enum inputs { UP = 72, DOWN = 80, SPACE = 32, W = 119, S = 115 };

			switch (_getch())
			{
			case UP:
			case W:
				if (targetElement != 0)
				{
					previousElement = targetElement;
					targetElement--;
					updateElement(previousElement, white);
					updateElement(targetElement, green);
				}
				break;

			case DOWN:
			case S:
				if (targetElement != dynamicText.size() - 1)
				{
					previousElement = targetElement;
					targetElement++;
					updateElement(previousElement, white);
					updateElement(targetElement, green);
				}
				break;

			case SPACE:
				inMenu = false;
				menuActions(targetElement);
				break;
			}
		}
	}
}
