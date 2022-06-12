#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY				0.0006f
#define GOOMBA_WALKING_SPEED		0.05f
#define GOOMBA_DIE_BOUNCE_SPEED		0.3f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT	500

// In case goomba is hit by deadly attacks and bounces, wait for it a bit before deleting it
#define GOOMBA_DIE_DELAY	3000

#define GOOMBA_STATE_WALKING				100
#define GOOMBA_STATE_DIE					200
#define GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS	300

#define ID_ANI_GOOMBA_WALKING					5111
#define ID_ANI_GOOMBA_DIE						5112
#define ID_ANI_GOOMBA_HIT_BY_DEADLY_ATTACKS		5113

#define GOOMBA_SCORES_GIVEN_WHEN_HIT 100

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return GOOMBA_SCORES_GIVEN_WHEN_HIT; }
	void SetDirectionX(int nx) { this->nx = nx; }
};