#pragma once

#include "MushroomBig.h"

#define ID_ANI_MUSHROOM_UP_MOVING	4120	
#define MUSHROOM__UP_LIVES_GIVEN_WHEN_HIT 1
#define MUSHROOM__UP_SCORES_GIVEN_WHEN_HIT 0

class CMushroomUp : public CMushroomBig
{
protected:
	virtual void Render();
public:
	CMushroomUp(float x, float y) : CMushroomBig(x, y) {};
	virtual int GetLivesGivenWhenHit() { return MUSHROOM__UP_LIVES_GIVEN_WHEN_HIT; }
	virtual int GetScoresGivenWhenHit() { return MUSHROOM__UP_SCORES_GIVEN_WHEN_HIT; }
};

