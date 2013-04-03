#include "Character.h"

Character::Character(void)
{
	 WalkRight = true;
     WalkBack = false;
     WalkLeft = false;
     WalkFront = false;
     Stopped = true;
     Walk = false;
	 Speed = 1;
}

Character::~Character(void)
{
}

void Character::SetRight()
{
	WalkRight = true;
	WalkLeft = false;
	WalkBack = false;
	WalkFront = false;
}

void Character::SetLeft()
{
	WalkRight = false;
	WalkLeft = true;
	WalkBack = false;
	WalkFront = false;
}

void Character::SetFront()
{
	WalkRight = false;
	WalkLeft = false;
	WalkBack = false;
	WalkFront = true;
}

void Character::SetBack()
{
	WalkRight = false;
	WalkLeft = false;
	WalkBack = true;
	WalkFront = false;
}

void Character::Draw(int frame)
{
	person.draw(frame);
}
