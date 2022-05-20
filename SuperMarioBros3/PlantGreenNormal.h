#pragma once

#include "GameObject.h"
#include "debug.h"

#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"


class CPlayScene;
class CMario;

#define	PLANT_RENDER_PRIORITY	10

#define PLANT_BBOX_WIDTH 16
#define PLANT_BBOX_HEIGHT 23

#define PLANT_MOVING_SPEED 0.07f

#define ID_ANI_PLANT_GREEN_NORMAL			5331

#define PLANT_SCORES_GIVEN_WHEN_HIT 100

#define PLANT_STATE_HIDING			100
#define PLANT_STATE_APPEARING		200
#define PLANT_STATE_DISAPPEARING	500

#define PLANT_APPEARING_ZONE_MAX	160
#define PLANT_APPEARING_ZONE_MIN	30

#define PLANT_TIME_BETWEEN_APPEARANCES	1500

// Time between when the plant appears until it disappears
#define PLANT_APPEARING_TIMEOUT			2000

class CPlantGreenNormal : public CGameObject
{
protected:
	ULONGLONG appear_start;
	ULONGLONG hide_start;

	int relative_nx_to_mario;
	int relative_ny_to_mario;

	float fireshot_dest_x, fireshot_dest_y; // To tell the fireshot which angle to shoot

	BOOLEAN isMarioInFireZone;
	float appearing_destination_y;
	float disappearing_destination_y;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual bool IsHidden() { return (state == PLANT_STATE_HIDING); }

public:
	CPlantGreenNormal(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return PLANT_SCORES_GIVEN_WHEN_HIT; }

	// So that plant will be hidden by pipe
	virtual int GetRenderPriority() { return PLANT_RENDER_PRIORITY; }
};

