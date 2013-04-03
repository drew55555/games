#pragma once
#include "Character.h"
class Objects
{
public:
	float x, y, width, height;
	bool IsWithin(float Cx, float Cy);
	Objects(void);
	Objects(float x1, float y1, float width1, float height1);
	~Objects(void);
};

