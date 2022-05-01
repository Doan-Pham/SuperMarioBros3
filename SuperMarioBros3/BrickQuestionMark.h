#pragma once

#include "Brick.h"
#include "Mario.h"
#include "debug.h"

#define ID_ANI_BRICK_QUESTIONMARK 10000
#define ID_ANI_TEST 11000
#define BRICK_QUESTION_MARK_GRAVITY 0.002f
#define BRICK_QUESTION_MARK_BOUNCE_SPEED 0.8f
class CBrickQuestionMark : public CBrick
{
protected:
	bool isContentTaken;
	float y_original;
	float ay;
public:
	CBrickQuestionMark(float x, float y) : CBrick(x, y) 
	{
		isContentTaken = false; ay = 0; y_original = this->y;
	}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);

	virtual int IsCollidable() { return 1; };


};

