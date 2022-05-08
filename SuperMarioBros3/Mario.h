#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "PMeter.h"

#include "debug.h"

class CPlayScene;
typedef CPlayScene* LPPLAYSCENE;


// TODO: Change the time of each frame for mario's walking animation to
// make mario move faster when p-meter is not fully charged

// TODO: Make the framtime for mario running animation shorter to create an illusion of him 
// moving quick

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#pragma region MARIO_STATE

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLY				700
#pragma endregion


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT					1112
#define ID_ANI_MARIO_IDLE_LEFT					1111

#define ID_ANI_MARIO_WALKING_RIGHT				1122
#define ID_ANI_MARIO_WALKING_LEFT				1121

#define ID_ANI_MARIO_RUNNING_RIGHT				1132
#define ID_ANI_MARIO_RUNNING_LEFT				1131

#define ID_ANI_MARIO_JUMP_WALK_RIGHT			1142
#define ID_ANI_MARIO_JUMP_WALK_LEFT				1141

#define ID_ANI_MARIO_JUMP_RUN_RIGHT				1192
#define ID_ANI_MARIO_JUMP_RUN_LEFT				1191

#define ID_ANI_MARIO_SIT_RIGHT					1152
#define ID_ANI_MARIO_SIT_LEFT					1151

#define ID_ANI_MARIO_BRACE_RIGHT				1162
#define ID_ANI_MARIO_BRACE_LEFT					1161

//#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 400

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT			1312
#define ID_ANI_MARIO_SMALL_IDLE_LEFT			1311

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT		1322
#define ID_ANI_MARIO_SMALL_WALKING_LEFT			1321

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT		1332
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT			1331

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT			1361
#define ID_ANI_MARIO_SMALL_BRACE_LEFT			1362

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT		1342
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT		1341

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT		1392
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT		1391



#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT			1512
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT			1511

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT		1522
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT		1521

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT		1532
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT		1531

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT	1542
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT		1541

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT		1592
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT		1591

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT			1552
#define ID_ANI_MARIO_RACCOON_SIT_LEFT			1551

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT		1562
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT			1561


#pragma endregion

#define GROUND_Y 160.0f


// Mario has many levels like big, tanooki, raccoon,... which are defined as 1, 2, 3, 4
// but tanooki or frog or fire is not exactly higher level than raccoon. The above numbers are
// are just for identification, not the level's actual value. The way this baseline works is to
// group levels according to their actual values: 
//	+ Levels lower than baseline: Low level
//	+ Equal: Mid level
//	+ Higher: High level
#define MARIO_LEVELS_BASELINE 2

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON 3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_UNTOUCHABLE_TIME 2500

#define BLOCK_PUSH_FACTOR_GHOST_PLATFORM 1.0f

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;

	LPPMETER pMeter;

	// *** CONST *** pointer to the current playscene
	const LPPLAYSCENE currentScene;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithItem(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

public:
	CMario(float x, float y, const LPPLAYSCENE& currentScene)
		: CGameObject(x, y), currentScene(currentScene)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;

		pMeter = new CPMeter();
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable() { return (state != MARIO_STATE_DIE); }

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	// This is so that mario will always be the first object in the vector "objects"
	virtual int GetRenderPriority() { return 99999999; }

	bool IsPMeterFullyCharged(){ return pMeter->isFullyCharged(); }
	void IncreasePMeter() { pMeter->SetState(P_METER_STATE_INCREASING); }
};