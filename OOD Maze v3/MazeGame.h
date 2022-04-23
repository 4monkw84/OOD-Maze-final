#pragma once

#include <map> // for std::map
#include <vector> // for std::vector
#include <chrono> // for clock_t, clock()

#include "cMaze.h"
#include "Renderer.h"
#include "Player.h"
#include "Object.h"
#include "Coin.h"
#include "Enemy.h"
#include "Medkit.h"
#include "Exit.h"

class MazeGame : public Renderer, private cMaze
{
private:
	// Some console colour codes for better maze visibility
	enum Colours { white = 15, yellow = 14, pink = 13, red = 12, aqua = 11, green = 10 };

	// Player stats
	bool atExit = false;
	int score = 0;

	// offsets for centre aligning maze and objects
	const short drawOffsetX = 30;
	const short drawOffsetY = 4;

	// Struct to store object variables for printing of guide and creation of objects
	struct objectAttributes
	{
		const char*	 name;
		const char	 character;
		const int	 colour;
		const int	 scoreValue;
		const int	 maxQuantity;
	};

	objectAttributes	a_player{ "Player", (char)2, pink, 0, 1 };
	objectAttributes	a_coin{ "Coin", (char)184, yellow, 25, 25 };
	objectAttributes	a_enemy{ "Enemy", (char)4, red, 50, 10 };
	objectAttributes	a_medkit{ "Medkit", (char)3, green, 0, 5 };
	objectAttributes	a_exit{ "Exit", (char)31, aqua, 250, 1 };

	std::vector<objectAttributes> objAttrs{ a_player, a_coin, a_enemy, a_medkit, a_exit };

	std::map<int, std::shared_ptr<Object>> m_objects;
	std::shared_ptr<Player> p_player = nullptr;

	// object map functions
	int		 generateMapKey(int x, int y);

	// maze creation functions
	void	 spawnPlayer();	 
	void	 spawnCoins();	
	void	 spawnEnemies(); 
	void	 spawnMedkits();
	void	 spawnExit();
	void	 initObjects();

	// drawing functions
	void	 drawObject(std::shared_ptr <Object> obj);
	void     drawFloor(int x, int y);
	void	 drawAllObjects();
	void	 drawGuide();
	void	 drawStats();

	// object movement functions
	void	 moveObject(std::shared_ptr <Object> obj, int x, int y);
	void	 moveObject(std::shared_ptr <Object> obj, cVector2 pos);

	// player specific functions
	bool	 playerCollision(int x, int y);
	void	 movementInput(int);

	// enemy specific functions
	void	 enemyMovement();
	void	 enemyPathfinding();

	// core functions
	void	 gameLoop();
	bool	 gameActive();

public:
	void startGame(); 
	void resetGame();
	int getScore() { return score; }
	int getPlayerHealth() { return p_player->getHealth(); }
	~MazeGame();
};

