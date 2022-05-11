#pragma once

#include "GameObject.h"
#include "debug.h"

#define	PLANT_RENDER_PRIORITY	10

#define PLANT_BBOX_WIDTH 16
#define PLANT_BBOX_HEIGHT 32

#define PLANT_MOVING_SPEED 0.02f

#define ID_SPRITE_PLANT_RED_FIRE_OPEN_TOP_LEFT				53211
#define ID_SPRITE_PLANT_RED_FIRE_OPEN_TOP_RIGHT				53221
#define ID_SPRITE_PLANT_RED_FIRE_OPEN_BOTTOM_LEFT			53231
#define ID_SPRITE_PLANT_RED_FIRE_OPEN_BOTTOM_RIGHT			53241

#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_LEFT			5321
#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_RIGHT			5322
#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_BOTTOM_LEFT		5323
#define ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_BOTTOM_RIGHT		5324

#define PLANT_SCORES_GIVEN_WHEN_HIT 100

#define PLANT_STATE_HIDING			100
#define PLANT_STATE_APPEARING		200
#define PLANT_STATE_FIRING			300
#define PLANT_STATE_DISAPPEARING	400



class CPlantRedFire : public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	int GetPlantAniSpriId();

	virtual int IsBlocking() { return 0; }
	virtual bool IsHidden() { return (state == PLANT_STATE_HIDING); }

public:
	CPlantRedFire(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return PLANT_SCORES_GIVEN_WHEN_HIT; }

	// To be hidden by the pipe
	virtual int GetRenderPriority() { return PLANT_RENDER_PRIORITY; }
};

