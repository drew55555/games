#pragma once
#include <string>
#include "Objects.h"
#include "Interior.h"
class Interior;

using namespace std;

class Location
{
private:
	string filename;
	int distance(float x1, float y1, float x2, float y2);
public:
	Location *top, *bottom, *right, *left;
	Objects *objects;
	Interior *interiors;
	CObject *npcs;
	int ObjectNum, InteriorNum, NPCNum;
	Location(void);
	Location(string name, Location *up, Location *down, Location *Right, Location *Left, int osize, Objects ObjectArray[], int isize,
		Interior intarray[], int npcSize, CObject cobjectarray[]);
	~Location(void);
	string getName(){return filename;}
	bool checkInterior(Character &character);
	void InsertTop(string name, Location *up, Location *Right, Location *Left, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[]);
	void InsertRight(string name, Location *up, Location *Down, Location *Right, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[]);
	void InsertLeft(string name, Location *up, Location *Left, Location *Down, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[]);
	void InsertBottom(string name, Location *Down, Location *Right, Location *Left, int size, Objects ObjectArray[], int isize, Interior intarray[], int npcSize, CObject cobjectarray[]);
};

