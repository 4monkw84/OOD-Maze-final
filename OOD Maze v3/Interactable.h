#pragma once

#include "Object.h"

class Interactable
{
protected:
	const int scoreValue;

public:
	Interactable(int scoreValue);
	virtual ~Interactable();
};