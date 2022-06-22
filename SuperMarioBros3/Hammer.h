#pragma once

#include "GameObject.h"

#define HAMMER_GRAVITY	0.0006f

#define	HAMMER_SPEED_X	0.08f;
#define	HAMMER_SPEED_Y	0.25f;

#define HAMMER_BBOX_WIDTH	8
#define HAMMER_BBOX_HEIGHT	14

#define ID_ANI_HAMMER_MOVING	8120

// Fireball is thrown by mario_fire that bounces off blocking objects and kill enemies

class CHammer : public CGameObject
{
protected:
	int nx; //hammer x direction
	float ay;

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	int IsSlippingThroughBlocks() { return 1; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CHammer(float x, float y, int nx);
};

