#pragma once

#include "Block.h"
#include "Item.h"
#include "debug.h"

#define ID_ANI_BRICK_GLASS_NORMAL 3121
#define ID_ANI_BRICK_GLASS_HIT 3122

#define BRICK_GLASS_GRAVITY 0.002f
#define BRICK_GLASS_BOUNCE_SPEED 0.3f

#define BRICK_STATE_NORMAL 100
#define BRICK_STATE_HIT_BY_MARIO 200
#define BRICK_STATE_GIVE_CONTENT 300

#define BRICK_GLASS_SCORES_GIVEN_WHEN_HIT 10

class CBrickGlass : public CBlock
{
protected:
	float y_original;
	float ay;

	CItem* hiddenItem;

	bool isHidingUpMushroom;
	bool isHidingPBlock;

	// This variable is to stop mario from making the brick bounce after its content is
	// already given
	bool isHitByMario;

	bool isContentGiven;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	// Lower than items to cover them
	virtual int GetRenderPriority() { return 20; }

public:
	CBrickGlass(float x, float y, bool isHidingUpMushroom, bool isHidingPBlock);

	virtual int GetScoresGivenWhenHit()
	{
		if (hiddenItem == nullptr) return 0;
		return BRICK_GLASS_SCORES_GIVEN_WHEN_HIT;
	}
	virtual void SetState(int state);
	virtual bool IsHidingUpMushroom() { return isHidingUpMushroom; }
	bool IsHidingPBlock() { return isHidingPBlock; }

	virtual void AddHiddenItem(CItem* item) { this->hiddenItem = item; }
};

