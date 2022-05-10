#pragma once

#include "GameObject.h"
#include "debug.h"

#define PLANT_BBOX_WIDTH 16
#define PLANT_BBOX_HEIGHT 32


#define ID_SPRITE_PLANT_RED_FIRE_OPEN_MOUTH_TOP_LEFT		53211
#define ID_SPRITE_PLANT_RED_FIRE_OPEN_MOUTH_TOP_RIGHT		53221
#define ID_SPRITE_PLANT_RED_FIRE_OPEN_MOUTH_BOTTOM_LEFT		53231
#define ID_SPRITE_PLANT_RED_FIRE_OPEN_MOUTH_BOTTOM_RIGHT	53241

#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_LEFT			5321
#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_RIGHT			5322
#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_BOTTOM_LEFT		5323
#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_BOTTOM_RIGHT		5324

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

