#pragma once

#include "GameObject.h"

#define	FIREBALL_SPEED_X	(0.06f);

#define FIREBALL_BBOX_WIDTH	8
#define FIREBALL_BBOX_HEIGHT	8

#define ID_ANI_FIREBALL_MOVING	8110
class CFireBall : public CGameObject
{
protected:
	int nx, ny; //fire ball direction
	int destination_x, destination_y; //mario's position

public:
	CFireBall(float x, float y, int destination_x, int destination_y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

