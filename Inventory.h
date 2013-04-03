#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H
#include "Character.h"

class Inventory
{
public:
	void *font2;
	float color[3];
	bool on;
	CObject box;
	void draw() {box.draw(0); textPrintf(110/512.0, 390/512.0, font2, "Hello", color);};
	Inventory(void);
	~Inventory(void);
};

#endif