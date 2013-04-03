/*
Drew Dishman
CS335 Lab3
4/19/2012

Version 1:
There is a scrolling background and the character is walking continually to the right continuing from the left side when he exits the screen
Version 2:
  The character begins by walking right to the center of the town. He then turns and walks up to the top, then back down and to where he started.
  The variables added for this implementation are 6 boolean functions. Four for indicating the direction and two for letting the program know if
the character is walking or stopped. A constant float to indicate speed is also added as well as a timer for detecting how long the character has
been stopped and an integer to hold the initial time for this.
  There were five functions added. SetRight, SetLeft, SetFront,SetBack and DisplayDirection. The first four functions are used to set the boolean
variables to set which direction the character is facing. The DisplayDirection function is made to let the programmer pass 3 sprites that need to
be displayed during the current animation. This saves a lot of typing and makes the code a little more readable.
Version 3:
    The code has been changed so that it uses character objects rather than keeping track of everything with global variables. Also the user now moves
	the character with the w s d and a keys and exits with the escape key. One more feature that was added is that when the top of the screen is reached,
	an alternate background is loaded and the player moves to the bottom of the screen, allowing people to move to different environments.
*/

//#include "2DGraphics.h"
#include <cmath>
#include "Character.h"
#include "Location.h"
#include "Inventory.h"

#define EQUIP 0
#define MAGIC 1
#define ITEMS 2
#define STOPTIME 4

void display();
void ComposeFrame();
void DisplayDirection(int, int, int);
void keyboard(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void CreateWorld();
void SetFront();
void SetBack();
void SetRight();
void SetLeft();
void drawPeople();
void SpecialFunction(int key, int x, int y);
bool GeradMoveRight(float &x, float &y);
bool GeradMoveLeft(float &x, float &y);
bool GeradMoveBack(float &x, float &y);
bool GeradMoveFront(float &x, float &y);
bool CheckNPC();

int current_frame=0;
bool w = false, a = false, s = false, d = false, InInterior = false, InMenu = false, topInterior = false, text1 = false, text2 = false;
void *font2;
float color[3];

CTimer StopTime;
int last_time, StoppedTime, currentInterior, walkTimer;
CTimer *Timer=new CTimer();
Inventory inventory;

CObject *NPCs = NULL;
CObject Textbox;
Character Gerad;
BMPClass background; 
CBaseSprite *mario1sprite=NULL, *InventorySprite=NULL, *NPC1Sprite = NULL, *NPC2Sprite = NULL, *TextboxSprite = NULL;
Objects objects[9] = {Objects(0, 190, 40, 300), Objects(0, 0, 50, 100), Objects(0, 260, 200, 215), Objects(240, 390, 215, 215), Objects(285, 350, 45, 50),
Objects(80, 150, 60, 300), Objects(375, 300, 200, 200), Objects(250, 0, 75, 110), Objects(435, 0, 75,50)};
Location places("StartingPoint.bmp", NULL, NULL, NULL, NULL, 9, objects, 0, NULL, 0, NULL);
Location *currentLocation = &places;
Location *exitLocation = NULL;


void CreateObjects()
{
  int x=50, y=250, xspeed=-4, yspeed=0;
	Gerad.person.create(x, y, xspeed, yspeed, mario1sprite, Timer);
	inventory.box.create(50, 50, 0, 0, InventorySprite, Timer);
	NPCs = new CObject[2];
	NPCs[0].create(x, y, xspeed, yspeed, NPC1Sprite, Timer);
	NPCs[1].create(x + 250, y, xspeed, yspeed, NPC2Sprite, Timer);
	Textbox.create(50, 50, 0, 0, TextboxSprite, Timer);
	color[0] = 20;
	color[1] = 1;
	color[2] = 20;
	font2 = GLUT_BITMAP_TIMES_ROMAN_24;
} 

bool LoadImages()
{
  background.load(currentLocation->getName());
  background.loadGLTextures();

  int r=254, g=0, b=254, frameCount=12, frame=0;    // r,g,b is background color to be filtered, frameCount and frame number
  mario1sprite=new CBaseSprite(frameCount, background.getViewportWidth(), background.getViewportHeight());
  mario1sprite->loadFrame(frame,   "Gerad - Walk (Right)1.bmp",r, g, b);
  mario1sprite->loadFrame(frame+1,   "Gerad - Walk (Right)2.bmp",r, g, b);
  mario1sprite->loadFrame(frame+2, "Gerad - Walk (Right)3.bmp", r, g, b);
  mario1sprite->loadFrame(frame+3, "Gerad - Walk (Left)1.bmp", r, g, b);
  mario1sprite->loadFrame(frame+4, "Gerad - Walk (Left)2.bmp", r, g, b);
  mario1sprite->loadFrame(frame+5, "Gerad - Walk (Left)3.bmp", r, g, b);
  mario1sprite->loadFrame(frame+6, "Gerad - Walk (Front)1.bmp", r, g, b);
  mario1sprite->loadFrame(frame+7, "Gerad - Walk (Front)2.bmp", r, g, b);
  mario1sprite->loadFrame(frame+8, "Gerad - Walk (Front)3.bmp", r, g, b);
  mario1sprite->loadFrame(frame+9, "Gerad - Walk (Back)1.bmp", r, g, b);
  mario1sprite->loadFrame(frame+10, "Gerad - Walk (Back)2.bmp", r, g, b);
  mario1sprite->loadFrame(frame+11, "Gerad - Walk (Back)3.bmp", r, g, b);
  mario1sprite->loadGLTextures();
  InventorySprite = new CBaseSprite(1, background.getViewportWidth(), background.getViewportHeight());
  InventorySprite->loadFrame(frame, "bckgnd1.bmp", r, g, b);
  InventorySprite->loadGLTextures();
  NPC1Sprite = new CBaseSprite(1, background.getViewportWidth(), background.getViewportHeight());
  NPC1Sprite->loadFrame(frame, "Scholar1 (Front).bmp", r, g, b);
  NPC1Sprite->loadGLTextures();
  NPC2Sprite = new CBaseSprite(1, background.getViewportWidth(), background.getViewportHeight());
  NPC2Sprite->loadFrame(frame, "Old Woman (Front).bmp", r, g, b);
  NPC2Sprite->loadGLTextures();
  TextboxSprite = new CBaseSprite(1, background.getViewportWidth(), background.getViewportHeight());
  TextboxSprite->loadFrame(frame, "textbox.bmp", r, g, b);
  TextboxSprite->loadGLTextures();
  return true;
}

extern void init();


int main(int argc,char** argv)
{
	glutInit(&argc,argv);

	init();
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(KeyUp);
	glutSpecialFunc(SpecialFunction);
	glutIgnoreKeyRepeat(true);

	Timer->start();
	last_time=Timer->time();
	walkTimer = Timer->time();

    LoadImages(); 
    CreateObjects();
	CreateWorld();

	glutMainLoop();
	return 0;
}

void CreateWorld()
{
	Objects TownUpObjects[9] = {Objects(375, 300, 50, 150), Objects(185, 325, 80, 130), Objects(85, 195, 50, 155), Objects(0, -1, 101, 210), Objects(100, -1, 135, 185),
	Objects(245, -1, 75, 100), Objects(310, -1, 50, 150), Objects(380, -1, 100, 150), Objects(400, -1, 100, 175)};
	places.InsertTop("TownUP.bmp", NULL, NULL, NULL, 9, TownUpObjects, 0, NULL, 0, NULL);
	Objects TownExitObjects[11] = {Objects(-1, -1, 140, 100), Objects(85, -1, 50, 155), Objects(160, -1, 40, 155), Objects(240, -1, 90, 155), Objects(340, -1, 150, 155),
	Objects(370, -1, 175, 175), Objects(-1, 360, 125, 150), Objects(315, 300, 300, 300), Objects(400, 240, 100, 100), Objects(30, 210, 50, 50), Objects(270, 210, 50, 50)};
	places.InsertBottom("TownExit.bmp", NULL, NULL, NULL, 11, TownExitObjects, 0, NULL, 2, NPCs);
	Objects TownLeftObjects[9] = {Objects(240, 290, 75, 85), Objects(-1, -1, 100, 175), Objects(90, -1, 50, 210), Objects(150, 0, 90, 210), Objects(210, 0, 30, 260),
	Objects(-1, 290, 70, 200), Objects(210, 0, 50, 75), Objects(285, 0, 177, 75), Objects(400, 75, 60, 125)};
	places.InsertLeft("TownLeft.bmp", NULL, NULL, NULL, 9, TownLeftObjects, 0, NULL, 0, NULL);
	currentLocation = currentLocation->left;
	Objects LeftUpObjects[8] = {Objects(0, 0, 235, 210), Objects(0, 209, 100, 200), Objects(0, 280, 230, 200), Objects(170, 250, 50, 200), Objects(245, 0, 200, 210),
	Objects(365, 209, 100, 200), Objects(250, 290, 300, 160), Objects(250, 250, 60, 100)};
	currentLocation->InsertTop("TownLeftUp.bmp", NULL, NULL, NULL, 8, LeftUpObjects, 0, NULL, 0, NULL);
	currentLocation = places.bottom;
	Objects FirstForest[9] = {Objects(0, 0, 190, 275), Objects(0, 199, 165, 100), Objects(0, 298, 110, 150), Objects(0, 370, 140, 75), Objects(190, 270, 75, 180),
	Objects(250, 0, 250, 150), Objects(280, 0, 250, 200), Objects(330, 189, 250, 150), Objects(380, 300, 100, 150)};
	currentLocation->InsertBottom("FirstForest.bmp", NULL, NULL, NULL, 9, FirstForest, 0, NULL, 0, NULL);
	currentLocation = currentLocation->bottom;
	Objects SecondForest[6] = {Objects(0, 0, 170, 200), Objects(215, 0, 85, 200), Objects(170, 120, 45, 160), Objects(330, 0, 100, 465), Objects(220, 250, 45, 160),
	Objects(0, 300, 250, 250)};
	currentLocation->InsertBottom("SecondForest.bmp", NULL, NULL, NULL, 6, SecondForest, 0, NULL, 0, NULL);
	currentLocation = currentLocation->bottom;
	Objects caveObjects[4] = {Objects(-1, -1, 200, 500), Objects(100, 200, 130, 500), Objects(300, -1, 500, 500), Objects(250, 200, 500, 500)};
	Interior Cave[1] = {Interior(new Location("FirstCave.bmp", NULL, NULL, NULL, NULL, 4, caveObjects, 0, NULL, 0, NULL), 215, 150, 240, 60, SetFront)};
	Objects ThirdForest[5] = {Objects(0, 0, 200, 500), Objects(230, -1, 220, 200), Objects(250, 199, 140, 160), Objects(390, 300, 50, 150), Objects(199, 325, 35, 30)};
	currentLocation->InsertLeft("ThirdForest.bmp", NULL, NULL, NULL, 5, ThirdForest, 1, Cave, 0, NULL);
	currentLocation = currentLocation->left;
	Objects SecondCaveObjects[5] = {Objects(-1, -1, 200, 300), Objects(-1, -1, 231, 200), Objects(0, 298, 255, 200), Objects(250, -1, 500, 200),
	Objects(280, 100, 500, 500)};
	currentLocation->interiors[0].getLocation()->InsertBottom("SecondCave.bmp", NULL, NULL, NULL, 5, SecondCaveObjects, 0, NULL, 0, NULL);
	currentLocation = &places;
}

void SetFront()
{
	Gerad.SetFront();
}

void SetBack()
{
	Gerad.SetBack();
}

void SetRight()
{
	Gerad.SetRight();
}

void SetLeft()
{
	Gerad.SetLeft();
}

int distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool checkInterior(int num)
{
	float x, y;
	int x1, y1;
	Gerad.Getxy(x, y);
	currentLocation->interiors[num].getxy(x1, y1);
	if(distance(x, y, x1, y1) < 20)
		return true;
	return false;
}

void interiorLoop(float x, float y)
{
	for(int i = 0; i < currentLocation->InteriorNum; i++)
    {
			if(checkInterior(i))
			{
				InInterior = true;
				currentInterior = i;
				currentLocation->interiors[i].getstartxy(x, y);
				y += 20;
				Gerad.Setxy(x, y);
				currentLocation->interiors[i].setdirection();
				currentLocation->interiors[i].exit = currentLocation;
				exitLocation = currentLocation;
				currentLocation = currentLocation->interiors[i].getLocation();
				background.load(exitLocation->interiors[i].getLocation()->getName());
				background.loadGLTextures();
				return;
			}
	}
}

bool CheckForExit(float x, float y)
{
	float x1, y1;
	exitLocation->interiors[currentInterior].getstartxy(x1, y1);
	if(distance(x, y, x1, y1) < 20) return true;
	return false;
}

bool GeradMoveRight(float &x, float &y)
{
	x += Gerad.GetSpeed();
			if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			for(int i = 0;i < currentLocation->ObjectNum;i++)
			{
				if(currentLocation->objects[i].IsWithin(x, y))
				{
					x -= Gerad.GetSpeed();
					Gerad.SetStopStatus();
					Gerad.Setxy(x, y);
					return true;
				}
			}
		    
			if(x >= 462)
			{
				if(currentLocation->right != NULL)
				{
					currentLocation = currentLocation->right;
				    x = 0;
					Gerad.Setxy(x, y);
				    background.load(currentLocation->getName());
				    background.loadGLTextures();
				}
				else{x = 462; Gerad.SetStopStatus();}
			}

			if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			return true;
}

bool GeradMoveLeft(float &x, float &y)
{
	 x -= Gerad.GetSpeed();
			if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			for(int i = 0;i < currentLocation->ObjectNum;i++)
			{
				if(currentLocation->objects[i].IsWithin(x, y))
				{
					x += Gerad.GetSpeed();
					Gerad.SetStopStatus();
					Gerad.Setxy(x, y);
					return true;
				}
			}
			if(x <= 0)
			if(currentLocation->left != NULL)
			{
				    currentLocation = currentLocation->left;
				    x = 462;
				    background.load(currentLocation->getName());
				    background.loadGLTextures();
			}
			else {x = 0; Gerad.SetStopStatus();}
		    if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			return true;
}

bool GeradMoveFront(float &x, float &y)
{
	y += Gerad.GetSpeed();
			if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			for(int i = 0;i < currentLocation->ObjectNum;i++)
			{
				if(currentLocation->objects[i].IsWithin(x, y))
				{
					y -= Gerad.GetSpeed();
					Gerad.SetStopStatus();
					Gerad.Setxy(x, y);
					return true;
				}
			}
			if(y >= 462)
			{
			    if(currentLocation->bottom != NULL)
				{
				    currentLocation = currentLocation->bottom;
					if(InInterior)topInterior = true;
					InInterior = false;
				    y = 0;
				    background.load(currentLocation->getName());
				    background.loadGLTextures();
				}
				else
				{
					y = 462; Gerad.SetStopStatus();
				}
			}
		    if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			return true;
}

bool GeradMoveBack(float &x, float &y)
{
	y -= Gerad.GetSpeed();
			if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			for(int i = 0;i < currentLocation->ObjectNum;i++)
			{
				if(currentLocation->objects[i].IsWithin(x, y))
				{
					y += Gerad.GetSpeed();
					Gerad.SetStopStatus();
					Gerad.Setxy(x, y);
					return true;
				}
			}
			if(y <= 0)
			{
				if(currentLocation->top != NULL)
				{
				    currentLocation = currentLocation->top;
					if(topInterior)InInterior = true;
				    y = 462;
				    background.load(currentLocation->getName());
				    background.loadGLTextures();
				}
				else
				{
					y = 0;
					Gerad.SetStopStatus();
				}
			}
		    if(Timer->elapsed(walkTimer, STOPTIME))
			{
			    Gerad.Setxy(x, y);
				walkTimer = Timer->time();
			}
			return true;
}

bool ProcessFrame()
{ 
	if(InMenu)
	{
		return true;
	}
	float x, y;
	Gerad.Getxy(x, y);
	if(Gerad.GetWalk())
	{
		if(InInterior)
		{
			if(CheckForExit(x, y))
			{
				int x, y;
				exitLocation->interiors[currentInterior].getxy(x, y);
				y += 20;
				Gerad.Setxy(x, y);
				currentLocation = exitLocation;
				Gerad.SetFront();
				background.load(currentLocation->getName());
				background.loadGLTextures();
				InInterior = false;
				return true;
			}
		}
        interiorLoop(x, y);
	    if(Gerad.GetRight())
		{
	        return GeradMoveRight(x, y);
        }
		else if(Gerad.GetBack())
        {
	        return GeradMoveBack(x, y);
		}
	    else if(Gerad.GetFront())
		{
	        return GeradMoveFront(x, y);
        }
        else if(Gerad.GetLeft())
        {
    	   return GeradMoveLeft(x, y);
        }
	}

  return true;
}

void ComposeFrame()
{
  if(Timer->elapsed(last_time,200))
  {
    last_time=Timer->time();
    if(++current_frame>=4)current_frame=0;
  }

  ProcessFrame();

  glutPostRedisplay();
}


void display()
{
   ComposeFrame();
   background.drawGLbackground();
   if (!InMenu)
   {
	   if(text1)
	   {
		   Textbox.draw(0);
		   textPrintf(110/512.0, 420/512.0, font2, "Please save the princess.", color);
	   }
	   else if(text2)
	   {
		   Textbox.draw(0);
		   textPrintf(90/512.0, 420/512.0, font2, "I always knew that old wizard was no good.", color);
	   }
	   drawPeople();
       if(Gerad.GetWalk())
       {
           if(Gerad.GetRight())
           {
               DisplayDirection(0, 1, 2);
           }
           else if(Gerad.GetLeft())
           {
    	       DisplayDirection(3, 4, 5);
           }
           else if(Gerad.GetBack())
           {
	           DisplayDirection(9, 10, 11);
           }
           else if(Gerad.GetFront())
           {
	           DisplayDirection(6, 7, 8);
           }
       }
       else
       {
	       if(Gerad.GetRight())
           {
               DisplayDirection(0, 0, 0);
           }
           else if(Gerad.GetLeft())
           {
	           DisplayDirection(3, 3, 3);
           }
           else if(Gerad.GetBack())
           {
	           DisplayDirection(9, 9, 9);
           }
           else if(Gerad.GetFront())
           {
	           DisplayDirection(6, 6, 6);
           }
	   }
   }
   else
   {
	   inventory.draw();
   }

   glutSwapBuffers();
}

void drawPeople()
{
	int size = currentLocation->NPCNum;
	for(int i = 0; i < size; i++)
	{
		currentLocation->npcs[i].draw(0);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'w': case 'W':
		w = true;
		Gerad.SetBack();
		Gerad.SetWalkStatus();
		break;
	case 's': case 'S':
		s = true;
		Gerad.SetFront();
		Gerad.SetWalkStatus();
		break;
	case 'a': case 'A':
		a = true;
		Gerad.SetLeft();
		Gerad.SetWalkStatus();
		break;
	case 'd': case 'D':
		d = true;
		Gerad.SetRight();
		Gerad.SetWalkStatus();
		break;
	case 'i': case 'I':
		InMenu = !InMenu;
		break;
	case 13:
		CheckNPC();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void KeyUp(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'w': case 'W':
		w = false;
		if(!(a || s || d))Gerad.SetStopStatus();
		break;
	case's': case 'S':
		s = false;
		if(!(a || w || d))Gerad.SetStopStatus();
		break;
	case 'a': case 'A':
		a = false;
		if(!(w || s || d))Gerad.SetStopStatus();
		break;
	case 'd': case 'D':
		d = false;
		if(!(a || s || w))Gerad.SetStopStatus();
		break;
	}
}

void SpecialFunction(int key, int x, int y)
{
	switch(key)
	{
	    default:
		    break;
	}
}

bool CheckNPC()
{
	if(currentLocation->npcs != NULL)
	{
		if(text1 || text2) { text1 = false; text2 = false; return true; }
		float x1, y1, x2, y2;
		Gerad.Getxy(x1, y1);
		for(int i = 0; i < currentLocation->NPCNum;i++)
		{
			currentLocation->npcs[i].Getxy(x2, y2);
			if(distance(x1, y2, x2, y2) < 50)
			{
				switch(i)
				{
				    case 0:
					    text1 = true;
					    break;
				    case 1:
					    text2 = true;
					    break;
				    default:
					    break;
				}
				return true;
			}
		}
	}
	return false;
}

void DisplayDirection(int zero, int one, int two)
{
	switch (current_frame)
       {
       case 0:
		   Gerad.Draw(zero);
	       break;
       case 1:
		   Gerad.Draw(one);
	       break;
       case 2:
		   Gerad.Draw(zero);
	       break;
       case 3:
		   Gerad.Draw(two);
	       break;
       default:
		   Gerad.Draw(zero);
	       break;
       }
	return;
}