#pragma once

#include "Item.h"

#include "Animations.h"

#define ID_ANI_COIN 4410
#define ID_ANI_BRICK_GLASS_NORMAL 3121

#define BLOCK_BBOX_WIDTH	16
#define BLOCK_BBOX_HEIGHT	16

#define COIN_BBOX_WIDTH		10
#define COIN_BBOX_HEIGHT	16

#define COIN_SCORES_GIVEN_WHEN_HIT 50
#define COIN_COINS_GIVEN_WHEN_HIT 1

#define BRICK_GLASS_SCORES_GIVEN_WHEN_HIT 10
#define BRICK_GLASS_COINS_GIVEN_WHEN_HIT 0

#define COIN_STATE_NORMAL 100
#define COIN_STATE_BECOME_BRICK	200

#define BECOME_BRICK_TIMEOUT		20000

class CCoin : public CItem
{
protected:
	ULONGLONG become_brick_start;

public:
	CCoin(float x, float y) : CItem(x, y) 
	{ 
		SetState(COIN_STATE_NORMAL);
		become_brick_start = -1; 
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return state == COIN_STATE_BECOME_BRICK; }

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int GetScoresGivenWhenHit() 
	{
		if (state == COIN_STATE_NORMAL)return COIN_SCORES_GIVEN_WHEN_HIT;
		else return BRICK_GLASS_SCORES_GIVEN_WHEN_HIT;
	}
	virtual int GetCoinsGivenWhenHit() 
	{ 
		if (state == COIN_STATE_NORMAL)return COIN_COINS_GIVEN_WHEN_HIT;
		else return BRICK_GLASS_COINS_GIVEN_WHEN_HIT;
	}
	void SetState(int state);
};