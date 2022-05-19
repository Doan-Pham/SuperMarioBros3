#pragma once

#include "GameObject.h"
#include "Animations.h"

#define ID_ANI_GOOMBA_RED_WING_WALKING_NORMAL			5141
#define ID_ANI_GOOMBA_RED_WING_WALKING_CLOSED_WING		5142
#define ID_ANI_GOOMBA_RED_WING_HOPPING					5143
#define ID_ANI_GOOMBA_RED_WING_JUMPING					5144
#define ID_ANI_GOOMBA_RED_WING_DIE						5145

#define GOOMBA_RED_WING_STATE_WALKING_NORMAL			100
#define GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING		200
#define GOOMBA_RED_WING_STATE_HOPPING					300
#define GOOMBA_RED_WING_STATE_JUMPING					400
#define GOOMBA_RED_WING_STATE_DIE						500

#define GOOMBA_NORMAL_BBOX_WIDTH		16
#define GOOMBA_NORMAL_BBOX_HEIGHT		16

#define GOOMBA_CLOSED_WING_BBOX_WIDTH	20
#define GOOMBA_CLOSED_WING_BBOX_HEIGHT	19

#define GOOMBA_OPEN_WING_BBOX_WIDTH		20
#define GOOMBA_OPEN_WING_BBOX_HEIGHT	24

#define GOOMBA_DIE_BBOX_HEIGHT 8


#define GOOMBA_RED_WING_GRAVITY		0.0002f
#define GOOMBA_WALKING_SPEED	0.05f
#define GOOMBA_HOPPING_SPEED	0.02f
#define GOOMBA_JUMPING_SPEED	0.15f


#define GOOMBA_DIE_TIMEOUT 500

// Time from when goomba lands after jumping and walks (closed wing) until it hops
#define GOOMBA_WALK_CLOSED_WING_TIMEOUT 2000

// TIme from when goomba starts hopping until it jumps
#define GOOMBA_HOP_TIMEOUT 1500

#define GOOMBA_RED_WING_SCORES_GIVEN_WHEN_HIT			100

#define BLOCK_PUSH_FACTOR_GHOST_PLATFORM 1.0f

class CGoombaRedWing : public CGameObject
{
protected:
	float ay;

	ULONGLONG die_start;
	ULONGLONG landing_start;

	BOOLEAN isOnPlatform;
	BOOLEAN isStillHaveWing;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	float GetBBoxHeight();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e);

public:
	CGoombaRedWing(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return GOOMBA_RED_WING_SCORES_GIVEN_WHEN_HIT; }
};

