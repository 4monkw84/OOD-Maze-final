#pragma once

#include "Object.h"

class Interactable
{
protected:
	const int m_scoreValue;

public:
	Interactable(int scoreValue);
	virtual ~Interactable();
	virtual int getScore() { return m_scoreValue; }
};