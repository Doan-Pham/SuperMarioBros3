#pragma once

#include "Block.h"
#include "Item.h"
#include "debug.h"

#define ID_ANI_BRICK_QUESTIONMARK_NORMAL 3111
#define ID_ANI_BRICK_QUESTIONMARK_HIT 3112

#define BRICK_QUESTION_MARK_GRAVITY 0.002f
#define BRICK_QUESTION_MARK_BOUNCE_SPEED 0.3f

#define BRICK_STATE_NORMAL 100
#define BRICK_STATE_HIT_BY_MARIO 200
#define BRICK_STATE_GIVE_CONTENT 300

#define BRICK_SCORES_GIVEN_WHEN_HIT 100
#define BRICK_COINS_GIVEN_WHEN_HIT 1

class CBrickQuestionMark : public CBlock
{
protected:
	float y_original;
	float ay;

	CItem* hiddenItem;
	bool isHidingItem;

	// This variable is to stop mario from making the brick bounces again after it's already hit by
	// mario
	bool isHitByMario;

	bool isContentGiven;

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	// Lower than items to cover them
	int GetRenderPriority() { return 20; }

public:
	CBrickQuestionMark(float x, float y, bool isHidingItem);

	int GetScoresGivenWhenHit()
	{
		if (isHidingItem) return 0;
		return BRICK_SCORES_GIVEN_WHEN_HIT;
	}
	int GetCoinsGivenWhenHit()
	{
		if (isHidingItem) return 0;
		return BRICK_COINS_GIVEN_WHEN_HIT;
	}

	void SetState(int state);
	bool IsHidingItem() { return isHidingItem; }
	bool IsContentGiven() { return isContentGiven; }
	void AddHiddenItem(CItem* item) { this->hiddenItem = item; }
};

