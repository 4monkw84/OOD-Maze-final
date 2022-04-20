#pragma once

#include <map> // for std::map
#include <vector> // for std::vector
//#include <ctime> // for clock_t, clock()
#include <chrono> // for clock_t

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
	objectAttributes	a_coin{ "Coin", (char)184, yellow, 25, 25};
	objectAttributes	a_enemy{ "Enemy", (char)4, red, -50, 10 };
	objectAttributes	a_medkit{ "Medkit", (char)3, green, 0, 5 };
	objectAttributes	a_exit{ "Exit", getExitChar(), aqua, 250, 1 };

	std::vector<objectAttributes> objAttrs{ a_player, a_coin, a_enemy, a_medkit, a_exit };

	std::map<int, Object*> m_objects; // map of all objects in maze
	std::vector<Enemy*> v_enemies; // vector of enemies for enemy movement
	Player* p_player = nullptr;

	// object map functions
	int		 generateKey(int x, int y); // generates a key for map related functionality

	// maze creation functions
	void	 spawnPlayer();	 // creates player within the maze
	void	 spawnCoins();	 // creates coins within the maze
	void	 spawnEnemies(); // creates enemies within the maze
	void	 spawnMedkits(); // creates medkits within the maze
	void	 spawnExit();	 // creates the maze exit; while the exit char already exists within the maze, this pairs with objectAttributes to improve visibility and award score
	void	 initObjects();  // parent function for spawning objects

	// drawing functions
	void	 drawObject(Object* obj); // draw a given object
	void     drawFloor(short x, short y); // redraws floor tiles to stop ghosting
	void	 drawAllObjects(); // finds and draws every object in the map
	void	 drawGuide();
	void	 drawStats();

	// object movement functions
	void	 moveObject(Object* obj, int x, int y); // move a given object to X/Y
	void	 moveObject(Object* obj, cVector2 pos); // move a given object to pos

	// player specific functions
	bool	 playerCollision(int x, int y); // handling of all collisions for the player character
	void	 movementInput(int); // used for getting player input, supports WASD and arrow key input

	// enemy specific functions
	void	 enemyMovement(); // used to generate path enemy can move within the maze
	void	 enemyPathfinding(); // 

	// core functions
	void	 gameLoop();	 // core loop of all functions
	bool	 gameActive();	 // check to see if any win/failure conditions have been met

public:
	void startGame();
	int getScore() { return score; }
	int getHealth() { return p_player->getHealth(); }
	~MazeGame();
};

