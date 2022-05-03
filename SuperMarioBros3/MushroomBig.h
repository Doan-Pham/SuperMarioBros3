#pragma once

#include "Item.h"
#include "Goomba.h"

#define ID_ANI_MUSHROOM_MOVING 4110

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_MOVING_SPEED 0.06f
#define MUSHROOM_APPEARING_SPEED 0.01f

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_STATE_HIDING 100
#define MUSHROOM_STATE_APPEARING 200
#define MUSHROOM_STATE_MOVING 300


class CMushroomBig : public CItem
{
protected:
	float ax;
	float ay;
	bool isHidden;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroomBig(float x, float y);
	virtual void SetState(int state, int nx = 1);
	virtual int GetScoresGivenWhenHit() { return 1000; }
};

