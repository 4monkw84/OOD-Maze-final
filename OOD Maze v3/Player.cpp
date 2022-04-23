#include "Player.h"

Player::Player(int x, int y, char objChar, int objColour)
	: Character(x, y, objChar, objColour)
{
}

void Player::gainHealth()
{
	if (health < maxHealth)
		health++;
}

void Player::loseHealth()
{
	health--;
}