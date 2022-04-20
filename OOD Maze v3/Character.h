#pragma once

#include "Object.h"
#include "cMaze.h"

class Character : public Object
{
public:
	Character(int x, int y, char objChar, int objColour);
	virtual ~Character();
};