#pragma once

#include "GameObject.h"


#define PLANT_SCORES_GIVEN_WHEN_HIT 100

class CPlantRedFire : public CGameObject
{
protected:
	float ax;
	float ay;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }

public:
	CPlantRedFire(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return PLANT_SCORES_GIVEN_WHEN_HIT; }
};

