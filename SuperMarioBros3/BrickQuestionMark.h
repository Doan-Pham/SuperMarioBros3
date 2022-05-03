#pragma once

#include "Brick.h"
#include "Mario.h"
#include "debug.h"
#include "MushroomBig.h"

#define ID_ANI_BRICK_QUESTIONMARK_NORMAL 3111
#define ID_ANI_BRICK_QUESTIONMARK_HIT 3112

#define BRICK_QUESTION_MARK_GRAVITY 0.002f
#define BRICK_QUESTION_MARK_BOUNCE_SPEED 0.3f

#define BRICK_STATE_NORMAL 100
#define BRICK_STATE_HIT_BY_MARIO 200

class CBrickQuestionMark : public CBrick
{
protected:
	float y_original;
	float ay;

	bool isHiddenItemAppeared;
	vector<CItem*> hiddenItems;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual int IsCollidable() { return 1; };

public:
	CBrickQuestionMark(float x, float y);

	virtual int GetScoresGivenWhenHit() { return 100; }
	virtual int GetCoinsGivenWhenHit() { return 1; }

	virtual void SetState(int state);
	virtual void AddHiddenItem(CItem* item) { hiddenItems.push_back(item); }
};

