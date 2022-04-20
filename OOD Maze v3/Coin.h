#pragma once

#include "Object.h"
#include "Interactable.h"

class Coin : public Interactable, public Object
{
private:

public:
	Coin(int x, int y, char objChar, int objColour, int objValue);
};

