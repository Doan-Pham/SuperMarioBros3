#pragma once

#include "Block.h"
#include "Item.h"
#include "debug.h"

#define ID_ANI_BRICK_GLASS_NORMAL 3121
#define ID_ANI_BRICK_GLASS_HIT 3122
#define ID_ANI_COIN	4410

#define BRICK_GLASS_GRAVITY 0.002f
#define BRICK_GLASS_BOUNCE_SPEED 0.3f

#define BRICK_STATE_NORMAL 100
#define BRICK_STATE_HIT_BY_MARIO 200
#define BRICK_STATE_GIVE_CONTENT 300
#define BRICK_STATE_BECOME_COIN	400

#define BRICK_GLASS_SCORES_GIVEN_WHEN_HIT 10
#define COIN_SCORES_GIVEN_WHEN_HIT 50
#define COIN_COINS_GIVEN_WHEN_HIT 1

#define BECOME_COIN_TIMEOUT		5000

class CBrickGlass : public CBlock
{
protected:
	float y_original;
	float ay;

	ULONGLONG become_coin_start;

	CItem* hiddenItem;

	bool isHidingUpMushroom;
	bool isHidingPBlock;
	bool isBlocking = true;
	// This variable is to stop mario from making the brick bounce after its content is
	// already given
	bool isHitByMario;

	bool isContentGiven;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return isBlocking; }
	// Lower than items to cover them
	virtual int GetRenderPriority() { return 20; }

public:
	CBrickGlass(float x, float y, bool isHidingUpMushroom, bool isHidingPBlock);

	virtual int GetScoresGivenWhenHit()
	{
		if (hiddenItem != nullptr) return 0;
		if (state == BRICK_STATE_BECOME_COIN) return COIN_SCORES_GIVEN_WHEN_HIT;
		return BRICK_GLASS_SCORES_GIVEN_WHEN_HIT;
	}
	virtual int GetCoinsGivenWhenHit()
	{
		if (state == BRICK_STATE_BECOME_COIN) return COIN_COINS_GIVEN_WHEN_HIT;
		return 0;
	}
	virtual void SetState(int state);
	virtual bool IsHidingUpMushroom() { return isHidingUpMushroom; }
	bool IsHidingPBlock() { return isHidingPBlock; }
	bool IsHitByMario() { return isHitByMario; }
	bool IsTurnedToCoin() { return state == BRICK_STATE_BECOME_COIN; }
	virtual void AddHiddenItem(CItem* item) { this->hiddenItem = item; }
};

