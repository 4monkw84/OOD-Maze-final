#pragma once

#include "Object.h"
#include "Interactable.h"

class Medkit : public Object, public Interactable
{
public:
	Medkit(int x, int y, char objChar, int objColour, int scoreVal);
};

