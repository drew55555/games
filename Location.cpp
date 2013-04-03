#include "Location.h"


Location::Location(void)
{
}

Location::Location(string name, Location *up, Location *down, Location *Right, Location *Left, int osize, Objects ObjectArray[], int isize,
	Interior intarray[], int npcSize, CObject cobjectarray[])
{
	filename = name;
	top = up;
	bottom = down;
	right = Right;
	left = Left;
	ObjectNum = osize;
	InteriorNum = isize;
	NPCNum = npcSize;
	if(cobjectarray != NULL)
	{
		npcs = cobjectarray;
	}
	else
		npcs = NULL;
	if(ObjectArray != NULL)
	{
		objects = new Objects[osize];
		for(int i = 0; i < osize; i++)
			objects[i] = ObjectArray[i];
	}
	else
		objects = NULL;

	if(intarray != NULL)
	{
		interiors = new Interior[isize];
		for(int i = 0; i < isize; i++)
			interiors[i] = intarray[i];
	}
	else
		interiors = NULL;
}


Location::~Location(void)
{
}

void Location::InsertTop(string name, Location *up, Location *Right, Location *Left, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[])
{
	top = new Location(name, up, this, Right, Left, size, ObjectArray, isize, intarray, npcSize, cobjectarray);
}

void Location::InsertRight(string name, Location *up, Location *Down, Location *Right, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[])
{
	right = new Location(name, up, Down, Right, this, size, ObjectArray, isize, intarray, npcSize, cobjectarray);
}

void Location::InsertLeft(string name, Location *up, Location *Left, Location *Down, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[])
{
	left = new Location(name, up, Down, this, Left, size, ObjectArray, isize, intarray, npcSize, cobjectarray);
}

void Location::InsertBottom(string name, Location *Down, Location *Right, Location *Left, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[])
{
	bottom = new Location(name, this, Down, right, left, size, ObjectArray, isize, intarray, npcSize, cobjectarray);
}

int Location::distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool Location::checkInterior(Character &character)
{
	float x, y;
	int x1, y1;
	character.Getxy(x, y);
	interiors->getxy(x1, y1);
	for(int i = 0; i < InteriorNum; i++)
	{
		if(distance(x, y, x1, y1) < 10)
			return true;
	}
	return false;
}