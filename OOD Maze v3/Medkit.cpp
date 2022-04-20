#include "Medkit.h"

Medkit::Medkit(int x, int y, char objChar, int objColour, int scoreVal)
	: Object(x, y, objChar, objColour), Interactable(scoreVal)
{
}
