#pragma once
#include "GameObject.h"
#include "debug.h"

#define ATTACHED_BBOX_BBOX_WIDTH		16
#define ATTACHED_BBOX_BBOX_HEIGHT	16

#define BLOCK_PUSH_FACTOR_GHOST_PLATFORM 1.0f

#define ATTACHED_BBOX__NORMAL_GRAVITY 0.002f

#define ATTACHED_BBOX_STATE_MOVING	100
#define ATTACHED_BBOX_STATE_FALL	200

// This object will be attached to another object for some mechanisms (koopa_red uses this to turn
// back whenever it reaches the platform's edge), the mechanism of the attached_bbox itself is just
// like a normal goomba
class CAttachedBBox : public CGameObject
{
protected:

	float ay;
	float y_original;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e);

public:
	CAttachedBBox(float x, float y, float vx, float vy);
	virtual void SetState(int state);
	int GetState() { return this->state; }
};

