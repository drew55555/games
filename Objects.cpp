#include "Objects.h"


Objects::Objects(void)
{
}

Objects::Objects(float x1, float y1, float width1, float height1)
{
	x = x1;
	y = y1;
	width = width1;
	height = height1;
}


Objects::~Objects(void)
{
}

bool Objects::IsWithin(float Cx, float Cy)
{
	return (Cx > x && Cx < x + width && Cy > y && Cy < y + height);
}
