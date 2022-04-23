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
	maxSteps = step.size() - 1;
}

void Enemy::addToPath(int x, int y)
{
	cVector2 temp(x, y);
	step.push_back(temp);
}

cVector2 Enemy::getNextStep()
{
	if (currentStep == 0) // if at start of path
	{
		if (maxSteps == 0) // if path length = 0
			return step.front(); // get first element
		else
			return step.at(currentStep + 1); // else get next step
	}
	else if (currentStep == maxSteps)
		return step.at(maxSteps - 1); // if at end of path, next step is 2nd to last element;
	else if (movingForward)
		return step.at(currentStep + 1); // if moving forward, get next element
	else 
		return step.at(currentStep - 1); // else moving backward, get previous element
}

void Enemy::advanceStep()
{
	updateDirection();

	if (maxSteps != 0)
	{
		if (movingForward)
			currentStep++;
		else
			currentStep--;
	}
}

void Enemy::setLastTime(clock_t time)
{
	lastTime = time;
}
