#pragma once

#include "cVector2.h"

class Object
{
private:
	cVector2 position;
	const char objChar;
	const int colour;

public:
	Object(int x, int y, char objChar, int objColour);
	virtual ~Object();

	virtual char	getChar() { return objChar; }
	virtual int		getColour() { return colour; }
	virtual int		getX() { return position.x; }
	virtual int		getY() { return position.y; }
	virtual void	setX(int x) { position.x = x; }
	virtual void	setY(int Y) { position.y = Y; }
	virtual void	setPosition(cVector2 pos);
	cVector2&		getPosition() { return position; }
};