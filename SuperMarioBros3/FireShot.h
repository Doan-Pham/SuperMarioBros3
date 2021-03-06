#pragma once

#include "GameObject.h"

// The standard amount of time the fireshot takes to reach mario, vx will be adjusted based on this
#define	FIRESHOT_STANDARD_TIME_X	1000 // unit: milliseconds

#define FIRESHOT_BBOX_WIDTH	8
#define FIRESHOT_BBOX_HEIGHT	8

#define ID_ANI_FIREBALL_MOVING	8110

// Fire shot is the fire that plant_fire shoots at mario, this shot moves through all objects
// and damage mario
class CFireShot : public CGameObject
{
protected:
	int nx, ny; //fire ball direction
	float destination_x, destination_y; //mario's position

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
public:
	CFireShot(float x, float y, float destination_x, float destination_y);

};

