#include "Enemy.h"

void Enemy::initStats()
{
	randomEngine.seed((int)(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	maxDistance = ((randomEngine() % absMaxDistance) + absMinDistance);
	movementSpeed = ((randomEngine() % absMaxSpeed) + absMinSpeed);
}

void Enemy::updateDirection()
{
	if (currentStep == 0)
		movingForward = true;
	else if (currentStep == maxSteps)
		movingForward = false;
}


Enemy::Enemy(int x, int y, char objChar, int objColour, int scoreVal)
	: Character(x, y, objChar, objColour), Interactable(scoreVal)
{
	initStats();
}

Enemy::~Enemy()
{
}

void Enemy::setMaxSteps()
{
	maxSteps = step.size();
}

void Enemy::addToPath(int x, int y)
{
	cVector2 temp(x, y);
	step.push_back(temp);
}

void Enemy::move()
{
		updateDirection();
		if (movingForward)
		{
			currentStep++;
		}
		else
		{
			currentStep--;
		}
}

cVector2 Enemy::getStep()
{
	if (currentStep == maxSteps)
		return step.back();
	if (currentStep == 0)
		return step.front();
	return step.at(currentStep);
}

void Enemy::setLastTime(clock_t time)
{
	lastTime = time;
}
