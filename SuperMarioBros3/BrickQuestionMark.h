#pragma once

#include "Brick.h"
#include "Mario.h"
#include "Item.h"

#include "debug.h"

#define ID_ANI_BRICK_QUESTIONMARK_NORMAL 3111
#define ID_ANI_BRICK_QUESTIONMARK_HIT 3112

#define BRICK_QUESTION_MARK_GRAVITY 0.002f
#define BRICK_QUESTION_MARK_BOUNCE_SPEED 0.3f

#define BRICK_STATE_NORMAL 100
#define BRICK_STATE_HIT_BY_MARIO 200

#define BRICK_SCORES_GIVEN_WHEN_HIT 100
#define BRICK_COINS_GIVEN_WHEN_HIT 1

class CBrickQuestionMark : public CBrick
{
protected:
	float y_original;
	float ay;

	bool isHiddenItemAppeared;
	vector<CItem*> hiddenItems;
	int hiddenItemToDropIndex;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsCollidable() { return 1; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CBrickQuestionMark(float x, float y);

	virtual int GetScoresGivenWhenHit() 
	{
		if (hiddenItems.size() > 0) return 0;
		return BRICK_SCORES_GIVEN_WHEN_HIT;
	}
	virtual int GetCoinsGivenWhenHit() 
	{
		if (hiddenItems.size() > 0) return 0;
		return BRICK_COINS_GIVEN_WHEN_HIT;
	}

	virtual void SetState(int state);
	virtual void AddHiddenItem(CItem* item) { hiddenItems.push_back(item); }
};

