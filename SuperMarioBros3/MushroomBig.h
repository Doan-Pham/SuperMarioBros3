#pragma once

#include "Item.h"


#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_MOVING_SPEED 0.04f


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_BBOX_HEIGHT_DIE 7

#define MUSHROOM_STATE_WALKING 100
#define MUSHROOM_STATE_DIE 200

#define ID_ANI_MUSHROOM_WALKING 5000

class CMushroomBig : public CItem
{
protected:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroomBig(float x, float y) : CItem(x, y) {}
	virtual int GetScoresGivenWhenHit() { return 1000; }
};

