#pragma once

#include "GameObject.h"

#define KOOPA_NORMAL_GRAVITY 0.002f
#define KOOPA_NORMAL_WALKING_SPEED 0.05f

#define KOOPA_SCORES_GIVEN_WHEN_HIT 100

class CKoopaRedNormal : public CGameObject
{
protected:

	float ay;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopaRedNormal(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return KOOPA_SCORES_GIVEN_WHEN_HIT; }
};

