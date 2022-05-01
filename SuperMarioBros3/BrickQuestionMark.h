#pragma once

#include "Brick.h"
#include "Mario.h"
#include "debug.h"

#define ID_ANI_BRICK_QUESTIONMARK 10000
#define ID_ANI_TEST 11000
#define BRICK_QUESTION_MARK_GRAVITY 0.0002f
#define BRICK_QUESTION_MARK_BOUNCE_SPEED 0.1f
class CBrickQuestionMark : public CBrick
{
protected:
	bool isContentTaken;
	float ay;
public:
	CBrickQuestionMark(float x, float y) : CBrick(x, y) 
	{ isContentTaken = false; ay = 0; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);

	virtual int IsCollidable() { return 1; };


};

