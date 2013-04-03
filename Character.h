#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include "2DGraphics.h"

class Character
{
private:
	bool WalkRight;
    bool WalkBack;
    bool WalkLeft;
    bool WalkFront;
    bool Stopped;
    bool Walk;
	float x, y, Speed;
public:
	CObject person;
	Character(void);
	~Character(void);
	float GetSpeed(){return Speed;}
	void SetRight();
	bool GetRight(){return WalkRight;}
    void SetLeft();
	bool GetLeft(){return WalkLeft;}
    void SetFront();
	bool GetFront(){return WalkFront;}
    void SetBack();
	bool GetBack(){return WalkBack;}
	void Getxy(float &x, float &y){person.Getxy(x, y);}
	void Setxy(float x, float y){person.Setxy(x, y);}
	void SetWalkStatus(){Walk = true; Stopped = false;}
	void SetStopStatus(){Walk = false; Stopped = true;}
	bool GetStopped() {return Stopped;}
	bool GetWalk(){return Walk;}
	void Draw(int frame);
};

#endif

