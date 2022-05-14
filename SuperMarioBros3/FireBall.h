#pragma once

#include "GameObject.h"

#define FIREBALL_GRAVITY	0.0006f

#define	FIREBALL_SPEED_X	0.15f;
#define	FIREBALL_SPEED_Y	0.15f;

#define FIREBALL_BBOX_WIDTH	8
#define FIREBALL_BBOX_HEIGHT	8

#define ID_ANI_FIREBALL_MOVING	8110

// Fireball is thrown by mario_fire that bounces off blocking objects and kill enemies

class CFireBall : public CGameObject
{
protected:
	int nx; //fire ball direction
	float ay;

	bool isDestroyed;

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CFireBall(float x, float y, int nx);
	bool IsDestroyed() { return isDestroyed; }
};

