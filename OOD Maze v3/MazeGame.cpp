#include "MazeGame.h"

#include <Windows.h> // for HANDLE, SetConsoleCursorPosition, SetConsoleTextAttribute
#include <conio.h> // for _getch()
#include <iostream> // for printf()
#include <chrono> // for clock()
#include <memory> // for std::shared_ptr, std::make_shared
#include <typeinfo> // for typeid()

//-----------------------------------------------------------------------------
//						   	 OBJECT MAP FUNCTIONS						     //
//-----------------------------------------------------------------------------
int MazeGame::generateMapKey(int x, int y) 
{
	return ((x + y) * (x + y + 1) / 2 + x);
}

//-----------------------------------------------------------------------------
//							MAZE CREATION FUNCTIONS						   	 //
//-----------------------------------------------------------------------------
void MazeGame::spawnPlayer()
{
	cVector2 targetPos;
	getStart(targetPos.x, targetPos.y);
	setPositionValue(targetPos.x, targetPos.y, a_player.character);

	int key = generateMapKey(targetPos.x, targetPos.y);
	p_player = std::make_shared<Player>(targetPos.x, targetPos.y, a_player.character, a_player.colour);
	m_objects.insert(std::make_pair(key, p_player));
}

//-----------------------------------------------------------------------------
void MazeGame::spawnCoins()
{
	cVector2 targetPos;

	for (int i = 0; i < a_coin.maxQuantity; i++)
	{
		getRandomEmptyPosition(targetPos.x, targetPos.y);
		m_objects.insert(std::make_pair(generateMapKey(targetPos.x, targetPos.y), std::make_shared<Coin>(targetPos.x, targetPos.y, a_coin.character, a_coin.colour, a_coin.scoreValue)));
		setPositionValue(targetPos.x, targetPos.y, a_coin.character);
	}
}

//-----------------------------------------------------------------------------
void MazeGame::spawnEnemies()
{
	cVector2 targetPos;

	for (int i = 0; i < a_enemy.maxQuantity; i++)
	{
		getRandomEmptyEndpoint(targetPos.x, targetPos.y);
		int key = generateMapKey(targetPos.x, targetPos.y); 
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(targetPos.x, targetPos.y, a_enemy.character, a_enemy.colour, a_enemy.scoreValue);
		setPositionValue(targetPos.x, targetPos.y, a_enemy.character); // set the location within the maze to enemy
		m_objects.insert(std::make_pair(key, enemy)); // insert enemy in object map
	}
}

//-----------------------------------------------------------------------------
void MazeGame::spawnMedkits()
{
	cVector2 targetPos;

	for (int i = 0; i < a_medkit.maxQuantity; i++)
	{
		getRandomEmptyPosition(targetPos.x, targetPos.y);
		m_objects.insert(std::make_pair(generateMapKey(targetPos.x, targetPos.y), std::make_shared<Medkit>(targetPos.x, targetPos.y, a_medkit.character, a_medkit.colour, a_medkit.scoreValue)));
		setPositionValue(targetPos.x, targetPos.y, a_medkit.character);
	}
}

//-----------------------------------------------------------------------------
void MazeGame::spawnExit()
{
	cVector2 targetPos;
	for (int i = 0; i < a_exit.maxQuantity; i++)
	{
		getExit(targetPos);
		m_objects.insert(std::make_pair(generateMapKey(targetPos.x, targetPos.y), std::make_shared<Exit>(targetPos.x, targetPos.y, a_exit.character, a_exit.colour, a_exit.scoreValue)));
		setPositionValue(targetPos.x, targetPos.y, a_exit.character);
	}
}

//-----------------------------------------------------------------------------
void MazeGame::initObjects()
{
	spawnPlayer();
	spawnExit();
	spawnCoins();
	spawnMedkits();
	spawnEnemies();
	enemyPathfinding();
}

//-----------------------------------------------------------------------------
//                            DRAWING FUNCTIONS                              //
//-----------------------------------------------------------------------------
void MazeGame::drawObject(std::shared_ptr<Object> obj)
{
	// set cursor position to object position, adjusting for draw offset
	gotoXY(obj->getX() + drawOffsetX, obj->getY() + drawOffsetY);
	setConsoleColour(obj->getColour());
	printf("%c", obj->getChar());
	resetCursor();
}

//-----------------------------------------------------------------------------
void MazeGame::drawFloor(int x, int y)
{
	gotoXY(x + drawOffsetX, y + drawOffsetY);
	printf("%c", getFloorChar());
	resetCursor();
}

//-----------------------------------------------------------------------------
void MazeGame::drawAllObjects()
{
	for (auto& [key, object] : m_objects)
	{
		drawObject(object);
	}
	resetCursor();
}

//-----------------------------------------------------------------------------
void MazeGame::drawGuide()
{
	short yOffset = 0;
	for (auto& attribs : objAttrs)
	{
		gotoXY(47 + drawOffsetX, drawOffsetY + yOffset);
		yOffset++;
		setConsoleColour(attribs.colour);
		printf("%s: %c", attribs.name, attribs.character);
	}
	resetCursor();
}

void MazeGame::drawStats()
{
	short posX = 47 + drawOffsetX;
	short posY = 19 + drawOffsetY;
	gotoXY(posX, posY);
	const int plrHP = p_player->getHealth();
	const int plrMaxHP = p_player->getMaxHealth();
	
	// Displaying player health as hearts that change colour based on health
	gotoXY(posX, posY);
	printf("Health: ");
	if (plrHP == plrMaxHP)
	{
		setConsoleColour(green);
		for (int i = 0; i < plrMaxHP; ++i)
		{
			printf("%c", 3);
		}
	}
	else if (plrHP == 1)
	{
		setConsoleColour(red);
		printf("%c     ", 3);
	}
	else
	{
		setConsoleColour(yellow);
		int difference = plrMaxHP - plrHP;
		for (int i = 0; i < plrHP; ++i)
		{
			printf("%c", 3);
		}
		for (int i = 0; i < difference; i++)
		{
			printf(" ");
		}
	}

	posY++;
	gotoXY(posX, posY);
	setConsoleColour(white);
	std::cout << "Score: " << score << "   ";
	resetCursor();
}

//-----------------------------------------------------------------------------
//                        OBJECT MOVEMENT FUNCTIONS                          //
//-----------------------------------------------------------------------------
void MazeGame::moveObject(std::shared_ptr <Object> obj, int x, int y)
{
	cVector2 curPos = obj->getPosition();
	drawFloor(curPos.x, curPos.y);
	setPositionValue(curPos.x, curPos.y, getFloorChar());
	m_objects.erase(generateMapKey(curPos.x, curPos.y));

	//sets new location
	setPositionValue(x, y, obj->getChar());
	obj->setX(x); 
	obj->setY(y); 
	m_objects.insert(std::make_pair(generateMapKey(x, y), obj)); 
}

//-----------------------------------------------------------------------------
void MazeGame::moveObject(std::shared_ptr <Object> obj, cVector2 pos)
{
	cVector2 curPos = obj->getPosition();
	drawFloor(curPos.x, curPos.y);
	setPositionValue(curPos.x, curPos.y, getFloorChar());
	m_objects.erase(generateMapKey(curPos.x, curPos.y)); 

	setPositionValue(pos.x, pos.y, obj->getChar()); 
	obj->setPosition(pos);
	m_objects.insert(std::make_pair(generateMapKey(pos.x, pos.y), obj));
}

//-----------------------------------------------------------------------------
//                       PLAYER SPECIFIC FUNCTIONS                           //
//-----------------------------------------------------------------------------
bool MazeGame::playerCollision(int x, int y) // Handles all possible collisions the player might face
{
	char collision = getPositionValue(x, y);

	if (collision == getWallChar())
	{
		return false;
	}

	else if (collision == getFloorChar())
	{
		return true;
	}

	else if (collision == a_exit.character)
	{
		score += a_exit.scoreValue;
		return true;
	}

	else if (collision == a_enemy.character)
	{
		score -= a_enemy.scoreValue;
		p_player->loseHealth();
		return false;
	}

	else if (collision == a_coin.character)
		score += a_coin.scoreValue;

	else if (collision == a_medkit.character)
		p_player->gainHealth();

	int key = generateMapKey(x, y);
	m_objects.erase(key);
	return true;
}

//-----------------------------------------------------------------------------
void MazeGame::movementInput(int key)
{
	int plrX = p_player->getX();
	int plrY = p_player->getY();

	enum inputs { UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77, W = 119, S = 115, A = 97, D = 100 };

	switch (key)
	{
	case UP: //Maze is dumb, up and down are inverted
	case W:
		if (playerCollision(plrX, plrY - 1))
		{
			moveObject(p_player, plrX, plrY - 1);
		}
		break;

	case DOWN: //Maze is dumb, up and down are inverted
	case S:
		if (playerCollision(plrX, plrY + 1))
		{
			moveObject(p_player, plrX, plrY + 1);
		}
		break;

	case LEFT:
	case A:
		if (playerCollision(plrX - 1, plrY))
		{
			moveObject(p_player, plrX - 1, plrY);
		}
		break;

	case RIGHT:
	case D:
		if (playerCollision(plrX + 1, plrY))
		{
			moveObject(p_player, plrX + 1, plrY);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
//                        ENEMY SPECIFIC FUNCTIONS                           //
//-----------------------------------------------------------------------------
void MazeGame::enemyMovement()
{
	clock_t currentTime = clock();
	clock_t lastTime;
	clock_t deltaTime;
	for (auto& [key, object] : m_objects)
	{
		if (typeid(*object) == typeid(Enemy))
		{
			std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(object);

			lastTime = enemy->getLastTime();
			deltaTime = currentTime - lastTime;

			if (deltaTime > enemy->getMoveSpeed())
			{
				cVector2 nextStep = enemy->getNextStep();

				if (getPositionValue(nextStep.x, nextStep.y) == a_player.character)
				{
					p_player->loseHealth();
					score -= a_enemy.scoreValue;
				}
				else
				{
					int x = enemy->getX(), y = enemy->getY();
					drawFloor(x, y);
					moveObject(enemy, enemy->getNextStep());
					enemy->advanceStep();
				}
				enemy->setLastTime(currentTime);
			}
		}
	}
}

void MazeGame::enemyPathfinding()
{
	cVector2 targetPos;
	for (auto& [key, object] : m_objects)
	{
		if (typeid(*object) == typeid(Enemy))
		{
			std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(object);

			targetPos = enemy->getPosition();
			enemy->addToPath(targetPos.x, targetPos.y);
			for (int i = 1; i < enemy->getMaxDistance(); i++) // offset by 1 to account for starting location
			{
				// check if up cell is empty, adding to path if it is
				if (getPositionValue(targetPos.x, targetPos.y - 1) == getFloorChar())
				{
					enemy->addToPath(targetPos.x, targetPos.y - 1);
					targetPos.y--;
				}
				// check if down cell is empty, adding to path if it is
				else if (getPositionValue(targetPos.x, targetPos.y + 1) == getFloorChar())
				{
					enemy->addToPath(targetPos.x, targetPos.y + 1);
					targetPos.y++;
				}
				//check if left cell is empty, adding to path if it is
				else if (getPositionValue(targetPos.x - 1, targetPos.y) == getFloorChar())
				{
					enemy->addToPath(targetPos.x - 1, targetPos.y);
					targetPos.x--;
				}
				// check if right cell is empty, adding to path if it is
				else if (getPositionValue(targetPos.x + 1, targetPos.y) == getFloorChar())
				{
					enemy->addToPath(targetPos.x + 1, targetPos.y);
					targetPos.x++;
				}
			}
			enemy->setMaxSteps();
		}
	}
}

//-----------------------------------------------------------------------------
//                            CORE GAME FUNCTIONS                            //
//-----------------------------------------------------------------------------
void MazeGame::gameLoop()
{
	COORD pos{ drawOffsetX, drawOffsetY - 1 };

	while (gameActive())
	{
		if (_kbhit())
		{
			movementInput(_getch());
		}
		enemyMovement();
		drawAllObjects();
		drawStats();
	}
}

//-----------------------------------------------------------------------------
bool MazeGame::gameActive()
{
	cVector2 exitPos;
	getExit(exitPos);

	if (p_player->getHealth() <= 0)
	{
		return false;
	}

	if (p_player->getPosition() == exitPos)
	{
		atExit = true;
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
void MazeGame::startGame()
{
	clearScreen();
	hideCursor();
	create(-1, 45, 21);
	print(drawOffsetX, drawOffsetY);
	initObjects();
	drawGuide();
	gameLoop();
}

void MazeGame::resetGame()
{
	m_objects.clear();
	score = 0;

	reset();
}

//-----------------------------------------------------------------------------
MazeGame::~MazeGame()
{
}