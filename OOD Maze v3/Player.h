#pragma once

#include "Character.h"

class Player : public Character
{
private:
	const int maxHealth = 3;
	int health = maxHealth;

public:
	Player(int x, int y, char objChar, int objColour);
	int getHealth() { return health; }
	int getMaxHealth() { return maxHealth; }
	void addHealth();
	void loseHealth();
};

