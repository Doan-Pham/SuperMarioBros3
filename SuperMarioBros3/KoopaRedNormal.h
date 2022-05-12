#pragma once

#include "GameObject.h"

#include "debug.h"

#define KOOPA_NORMAL_GRAVITY 0.002f
#define KOOPA_NORMAL_WALKING_SPEED 0.05f

#define ID_ANI_KOOPA_WALKING_LEFT					5231
#define ID_ANI_KOOPA_WALKING_RIGHT					5232

#define ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_LEFT		5233
#define ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_RIGHT		5234
#define ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_LEFT		5235
#define ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_RIGHT	5236

#define ID_ANI_KOOPA_SHELL_UPSIDE					5237
#define ID_ANI_KOOPA_SHELL_DOWNSIDE					5238

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

