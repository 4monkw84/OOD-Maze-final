#include "Object.h"

Object::Object(int x, int y, char objChar, int objColour)
	: m_char{ objChar }, m_colour{ objColour }
{
	position.x = x;
	position.y = y;
}

Object::~Object()
{
}

void Object::setPosition(cVector2 pos)
{
	position = pos;
}
