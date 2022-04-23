#pragma once

#include "Character.h"
#include "Interactable.h"
#include <vector> // for std::vector
#include <chrono> // for clock_t
#include <random> // for mt_19937

class Enemy : public Character, public Interactable
{
private:
	//random number generation
	std::mt19937 randomEngine;
	
	//pathfinding
	std::vector<cVector2> step;	    // vector of positions enemy can move to
	const int absMaxDistance = 5;	// upper bounds on distance enemy can pathfind
	const int absMinDistance = 3;	// lower bounds on distance enemy can pathfind
	int       maxDistance = 0;	    // number of steps enemy will find during pathfinding
	int		  maxSteps = 0;		    // number of steps enemy may make should it not find a path of length maxDistance

	//movement
	const int absMaxSpeed = 1000;   // upper bounds on how fast the enemy moves (in milliseconds)
	const int absMinSpeed = 500;	// lower bounds on how fast the enemy moves (in milliseconds)
	int		  movementSpeed = 0;    // actual movement speed of enemy (in milliseconds)
	int	      currentStep = 0;		// used for traversing step vector
	clock_t   lastTime = clock();   // used in calculating delta time for movement
	bool	  movingForward = true;

	//pathfinding
	void initStats();
	void updateDirection(); // check if enemy is at end step and reverse direction

public:
	Enemy(int x, int y, char objChar, int objColour, int scoreVal);
	~Enemy();

	//pathfinding
	int getMaxDistance() { return maxDistance; } // returns number of steps enemy can take
	void setMaxSteps(); // sets max steps to length of step vector in case of short movement range
	void addToPath(int x, int y); // adds location to step vector
	//movement
	cVector2 getNextStep();	  // gets the position of the next step in pathing for player collision detection
	void advanceStep();		  // advances step counter respective to direction moving in
	void setLastTime(clock_t time); // sets time since last update
	clock_t getLastTime() { return lastTime; } // returns last time since enemy moved
	int getMoveSpeed() { return movementSpeed; } // returns movement speed of enemy
};
