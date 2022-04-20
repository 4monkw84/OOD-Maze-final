#include "Coin.h"

Coin::Coin(int x, int y, char objChar, int objColour, int objValue)
	: Object(x, y, objChar, objColour), Interactable(objValue)
{
}