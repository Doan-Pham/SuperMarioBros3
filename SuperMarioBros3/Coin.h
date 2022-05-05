#pragma once

#include "Item.h"

#include "Animations.h"

#define ID_ANI_COIN 4410

#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_SCORES_GIVEN_WHEN_HIT 50
#define COIN_COINS_GIVEN_WHEN_HIT 1

class CCoin : public CItem
{
public:
	CCoin(float x, float y) : CItem(x, y) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int GetScoresGivenWhenHit() { return COIN_SCORES_GIVEN_WHEN_HIT; }
	virtual int GetCoinsGivenWhenHit() { return COIN_COINS_GIVEN_WHEN_HIT; }
};