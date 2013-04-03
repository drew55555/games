#include "Interior.h"


Interior::Interior(void)
{
}

Interior::Interior(Location *place, int x1, int y1, int startx1, int starty1, void (*SetDirection)())
{
	location = place;
	x = x1;
	y = y1;
	startx = startx1;
	starty = starty1;
	setdirection = SetDirection;
	exit = NULL;
}


Interior::~Interior(void)
{
}
