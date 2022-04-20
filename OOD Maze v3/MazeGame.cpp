#include "MazeGame.h"

#include <Windows.h> // for HANDLE, SetConsoleCursorPosition, SetConsoleTextAttribute
#include <conio.h> // for _getch()
#include <iostream> // for printf()
#include <chrono> // for clock()

//-----------------------------------------------------------------------------
//						   	 OBJECT MAP FUNCTIONS						     //
//-----------------------------------------------------------------------------
int MazeGame::generateKey(int x, int y) // generate a map key using the cantor pairing function to ensure no conflicts between objects
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

	int key = generateKey(targetPos.x, targetPos.y);
	p_player = new Player(targetPos.x, targetPos.y, a_player.character, a_player.colour);
	m_objects.insert(std::make_pair(key, p_player));
}

//-----------------------------------------------------------------------------
void MazeGame::spawnCoins()
{
	cVector2 targetPos;

	for (int i = 0; i < a_coin.maxQuantity; i++)
	{
		getRandomEmptyPosition(targetPos.x, targetPos.y);
		m_objects.insert(std::make_pair(generateKey(targetPos.x, targetPos.y), new Coin(targetPos.x, targetPos.y, a_coin.character, a_coin.colour, a_coin.scoreValue)));
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
		int key = generateKey(targetPos.x, targetPos.y); // generate key to store in object map
		Enemy* enemy = new Enemy(targetPos.x, targetPos.y, a_enemy.character, a_enemy.colour, a_enemy.scoreValue); // create new enemy
		setPositionValue(targetPos.x, targetPos.y, a_enemy.character); // set the location within the maze to enemy
		m_objects.insert(std::make_pair(key, enemy)); // insert enemy in object map
		v_enemies.push_back(enemy); // push enemy to vector
	}
}

//-----------------------------------------------------------------------------
void MazeGame::spawnMedkits()
{
	cVector2 targetPos;

	for (int i = 0; i < a_medkit.maxQuantity; i++)
	{
		getRandomEmptyPosition(targetPos.x, targetPos.y);
		m_objects.insert(std::make_pair(generateKey(targetPos.x, targetPos.y), new Medkit(targetPos.x, targetPos.y, a_medkit.character, a_medkit.colour, a_medkit.scoreValue)));
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
		m_objects.insert(std::make_pair(generateKey(targetPos.x, targetPos.y), new Exit(targetPos.x, targetPos.y, a_exit.character, a_exit.colour, a_exit.scoreValue)));
		setPositionValue(targetPos.x, targetPos.y, a_exit.character);
	}
}

//-----------------------------------------------------------------------------
void MazeGame::initObjects()
{
	spawnPlayer();
	spawnCoins();
	spawnEnemies();
	enemyPathfinding();
	spawnMedkits();
	spawnExit();
}

//-----------------------------------------------------------------------------
//                            DRAWING FUNCTIONS                              //
//-----------------------------------------------------------------------------
void MazeGame::drawObject(Object* obj)
{
	// set cursor position to object position, adjusting for draw offset
	gotoXY(obj->getX() + drawOffsetX, obj->getY() + drawOffsetY);
	setConsoleColour(obj->getColour());
	printf("%c", obj->getChar());
	resetCursor();
}

//-----------------------------------------------------------------------------
void MazeGame::drawFloor(short x, short y)
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
	for (size_t i = 0; i < objAttrs.size(); i++)
	{
		gotoXY(47 + drawOffsetX, drawOffsetY + i);
		setConsoleColour(objAttrs.at(i).colour);
		printf("%s: %c", objAttrs.at(i).name, objAttrs.at(i).character);
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
			std::cout << " ";
		}
	}

	// Displaying score
	gotoXY(posX, posY + 1);
	setConsoleColour(white);
	printf("Score: %i", score);
	resetCursor();
}

//-----------------------------------------------------------------------------
//                        OBJECT MOVEMENT FUNCTIONS                          //
//-----------------------------------------------------------------------------
void MazeGame::moveObject(Object* obj, int x, int y)
{
	int objX, objY;
	objX = obj->getX(); // gets current x coord
	objY = obj->getY(); // gets current y coord
	setPositionValue(objX, objY, getFloorChar()); //clears current cell in maze
	m_objects.erase(generateKey(objX, objY)); // erases map entry 

	//sets new location
	setPositionValue(x, y, obj->getChar()); // sets new location char to objects
	obj->setX(x); // sets object x to new location
	obj->setY(y); // sets object y to new location
	m_objects.insert(std::make_pair(generateKey(x, y), obj)); // creates new map entry with updated key
}

//-----------------------------------------------------------------------------
void MazeGame::moveObject(Object* obj, cVector2 pos)
{
	int objX, objY;
	objX = obj->getX(); // gets current x coord
	objY = obj->getY(); // gets current y coord
	setPositionValue(objX, objY, getFloorChar()); //clears current cell in maze
	m_objects.erase(generateKey(objX, objY)); // erases map entry 

	//sets new location
	setPositionValue(pos.x, pos.y, obj->getChar()); // sets new location char to objects
	obj->setX(pos.x); // sets object x to new location
	obj->setY(pos.y); // sets object y to new location
	m_objects.insert(std::make_pair(generateKey(pos.x, pos.y), obj)); // creates new map entry with updated key
}

//-----------------------------------------------------------------------------
//                       PLAYER SPECIFIC FUNCTIONS                           //
//-----------------------------------------------------------------------------
bool MazeGame::playerCollision(int x, int y) // Handles all possible collisions the player might face
{
	char collision = getPositionValue(x, y);

	if (collision == getWallChar())
		return false;

	if (collision == getFloorChar())
		return true;

	if (collision == a_exit.character)
	{
		score += a_exit.scoreValue;
		return true;
	}

	if (collision == a_enemy.character)
	{
		score += a_enemy.scoreValue;
		p_player->loseHealth();
		return false;
	}

	else if (collision == a_coin.character)
		score += a_coin.scoreValue;

	else if (collision == a_medkit.character)
		p_player->addHealth();

	int key = generateKey(x, y); // find object in map
	delete m_objects.at(key);	 // delete pointer in map
	m_objects.erase(key);		 // erase the object pointer from map
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
		if (playerCollision(plrX, plrY - 1) == true)
		{
			drawFloor(plrX, plrY);
			moveObject(p_player, plrX, plrY - 1);
		}
		break;

	case DOWN: //Maze is dumb, up and down are inverted
	case S:
		if (playerCollision(plrX, plrY + 1) == true)
		{
			drawFloor(plrX, plrY);
			moveObject(p_player, plrX, plrY + 1);
		}
		break;

	case LEFT:
	case A:
		if (playerCollision(plrX - 1, plrY) == true)
		{
			drawFloor(plrX, plrY);
			moveObject(p_player, plrX - 1, plrY);
		}
		break;

	case RIGHT:
	case D:
		if (playerCollision(plrX + 1, plrY) == true)
		{
			drawFloor(plrX, plrY);
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
	for (auto& enemy : v_enemies)
	{
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

void MazeGame::enemyPathfinding()
{
	cVector2 targetPos;
	//for (int i = 0; i < m_enemies.size(); i++)
	for (auto& enemy : v_enemies)
	{
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
	for (auto& [key, object] : m_objects)
	{
		delete object;
	}

	m_objects.clear();
	v_enemies.clear();
	score = 0;

	reset();
}

//-----------------------------------------------------------------------------
MazeGame::~MazeGame()
{
}