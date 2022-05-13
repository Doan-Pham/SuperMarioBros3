#pragma once

#include "GameObject.h"
#include "Animations.h"

#define ID_ANI_GOOMBA_RED_WING_WALKING					5141
#define ID_ANI_GOOMBA_RED_WING_WALKING_CLOSED_WING		5142
#define ID_ANI_GOOMBA_RED_WING_HOPPING					5143
#define ID_ANI_GOOMBA_RED_WING_HIGH_JUMP				5144
#define ID_ANI_GOOMBA_RED_WING_DIE						5145

#define GOOMBA_RED_WING_STATE_WALKING_NORMAL			100
#define GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING		200
#define GOOMBA_RED_WING_STATE_HOPPING					300
#define GOOMBA_RED_WING_STATE_JUMPING					400
#define GOOMBA_RED_WING_STATE_DIE						500

#define GOOMBA_BBOX_WIDTH	16
#define GOOMBA_BBOX_HEIGHT	16

#define GOOMBA_DIE_BBOX_HEIGHT 8


#define GOOMBA_GRAVITY	0.002f
#define GOOMBA_WALKING_SPEED 0.05f

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_RED_WING_SCORES_GIVEN_WHEN_HIT			100

class CGoombaRedWing : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CGoombaRedWing(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return GOOMBA_RED_WING_SCORES_GIVEN_WHEN_HIT; }
};

