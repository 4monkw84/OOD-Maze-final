#pragma once
#include "Object.h"
#include "Interactable.h"
class Exit : public Interactable, public Object
{
public:
	Exit(int x, int y, char objChar, int objColour, int objValue);
};

