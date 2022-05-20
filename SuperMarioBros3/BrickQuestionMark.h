#pragma once

#include "Brick.h"
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

class CBrickQuestionMark : public CBrick
{
protected:
	float y_original;
	float ay;

	CItem* hiddenItem;
	bool isHidingItem;
	bool isHidingUpMushroom;

	// This variable is to stop mario from making the brick bounce after its content is
	// already given
	bool isHitByMario;

	bool isContentGiven;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	// Lower than items to cover them
	virtual int GetRenderPriority() { return 20; }

public:
	CBrickQuestionMark(float x, float y, bool isHidingItem, bool isHidingUpMushroom);

	virtual int GetScoresGivenWhenHit()
	{
		if (hiddenItem == nullptr) return 0;
		return BRICK_SCORES_GIVEN_WHEN_HIT;
	}
	virtual int GetCoinsGivenWhenHit()
	{
		if (hiddenItem == nullptr) return 0;
		return BRICK_COINS_GIVEN_WHEN_HIT;
	}

	virtual void SetState(int state);
	virtual bool IsHidingItem() { return isHidingItem; }
	bool IsHidingUpMushroom() { return isHidingUpMushroom; }

	virtual void AddHiddenItem(CItem* item) { this->hiddenItem = item; }
};

