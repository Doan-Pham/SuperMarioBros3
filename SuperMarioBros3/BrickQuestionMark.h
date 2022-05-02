#pragma once

#include "Brick.h"
#include "Mario.h"
#include "debug.h"

#define ID_ANI_BRICK_QUESTIONMARK_NORMAL 3111
#define ID_ANI_BRICK_QUESTIONMARK_HIT 3112
#define BRICK_QUESTION_MARK_GRAVITY 0.002f
#define BRICK_QUESTION_MARK_BOUNCE_SPEED 0.3f

class CBrickQuestionMark : public CBrick
{
protected:
	bool isHitByMario;
	float y_original;
	float ay;
public:
	CBrickQuestionMark(float x, float y) : CBrick(x, y) 
	{
		isHitByMario = false; ay = 0; y_original = this->y;
	}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual int IsCollidable() { return 1; };

	virtual int GetScoresGivenWhenHit() { return 100; }
	virtual int GetCoinsGivenWhenHit() { return 1; }
};

