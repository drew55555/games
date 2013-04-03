#pragma once
#include "Location.h"
class Location;

class Interior
{
private:
	Location *location;
	int x, y, startx, starty;
public:
	Location *exit;
	void (*setdirection)();
	void getxy(int &x1, int &y1) {x1 = x; y1 = y;};
	void getstartxy(float &x1, float &y1) { x1 = startx; y1 = starty; };
	Location *getLocation() { return location; };
	Interior(void);
	Interior(Location *place, int x1, int y1, int startx1, int starty1, void (*SetDirection)());
	~Interior(void);
};

