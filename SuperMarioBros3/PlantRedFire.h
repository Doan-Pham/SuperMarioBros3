#pragma once

#include "GameObject.h"
#include "debug.h"

#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

#include "FireShot.h"

class CPlayScene;
class CMario;

#define	PLANT_RENDER_PRIORITY	10

#define PLANT_BBOX_WIDTH 16
#define PLANT_BBOX_HEIGHT 32

#define PLANT_MOVING_SPEED 0.07f

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
#define PLANT_STATE_AIMING			300
#define PLANT_STATE_FIRING			400
#define PLANT_STATE_DISAPPEARING	500

#define PLANT_APPEARING_ZONE_MAX	160
#define PLANT_APPEARING_ZONE_MIN	30

#define PLANT_FIRING_ZONE_MAX	(PLANT_APPEARING_ZONE_MAX - 30)

#define PLANT_TIME_BETWEEN_APPEARANCES	1500
#define PLANT_AIMING_TIMEOUT			1500
#define PLANT_FIRING_TIMEOUT			1500

class CPlantRedFire : public CGameObject
{
protected:

	ULONGLONG hide_start;
	ULONGLONG aim_start;
	ULONGLONG fire_start;

	int relative_nx_to_mario;
	int relative_ny_to_mario;

	float fireshot_dest_x, fireshot_dest_y; // To tell the fireshot which angle to shoot

	BOOLEAN isMarioInFireZone;
	float appearing_destination_y;
	float disappearing_destination_y;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetPositionRelativeToMario(int mario_x, int mario_y, int& nx, int& ny);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int GetPlantSpriteId();
	virtual int GetPlantAniId();

	virtual int IsBlocking() { return 0; }
	virtual bool IsHidden() { return (state == PLANT_STATE_HIDING); }

public:
	CPlantRedFire(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return PLANT_SCORES_GIVEN_WHEN_HIT; }

	// So that plant will be hidden by pipe
	virtual int GetRenderPriority() { return PLANT_RENDER_PRIORITY; }
};

