#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "PMeter.h"

#include "debug.h"

class CPlayScene;
typedef CPlayScene* LPPLAYSCENE;

class CKoopaRedNormal;

// TODO: Change the time of each frame for mario's walking animation to
// make mario move faster when p-meter is not fully charged



#pragma region MARIO_SPEED

#define MARIO_WALKING_SPEED			0.1f
#define MARIO_RUNNING_SPEED			0.15f

#define MARIO_TAKE_OFF_SPEED_Y		0.2f
#define MARIO_FLYING_SPEED_Y		0.13f

#define MARIO_ACCEL_WALK_X			0.0003f
#define MARIO_ACCEL_RUN_X			0.0005f

#define MARIO_JUMP_SPEED_Y			0.3f
#define MARIO_JUMP_RUN_SPEED_Y		0.35f

#define MARIO_GRAVITY				0.0006f
#define MARIO_GRAVITY_SLOW_FALL		0.0004f
#define MARIO_JUMP_DEFLECT_SPEED	0.25f

#pragma endregion 


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
#define MARIO_STATE_FALLING			800

#define MARIO_STATE_TAIL_WAGGING	900

#define MARIO_STATE_TAIL_WHIPPING	1000

#pragma endregion


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_LEFT					1111
#define ID_ANI_MARIO_IDLE_RIGHT					1112

#define ID_ANI_MARIO_WALKING_LEFT				1121
#define ID_ANI_MARIO_WALKING_RIGHT				1122

#define ID_ANI_MARIO_RUNNING_LEFT				1131
#define ID_ANI_MARIO_RUNNING_RIGHT				1132

#define ID_ANI_MARIO_JUMP_WALK_LEFT				1141
#define ID_ANI_MARIO_JUMP_WALK_RIGHT			1142

#define ID_ANI_MARIO_SIT_LEFT					1151
#define ID_ANI_MARIO_SIT_RIGHT					1152

#define ID_ANI_MARIO_BRACE_LEFT					1161
#define ID_ANI_MARIO_BRACE_RIGHT				1162

#define ID_ANI_MARIO_KICK_LEFT					1171
#define ID_ANI_MARIO_KICK_RIGHT					1172

#define ID_ANI_MARIO_HOLD_LEFT					1181
#define ID_ANI_MARIO_HOLD_RIGHT					1182

#define ID_ANI_MARIO_JUMP_RUN_LEFT				1191
#define ID_ANI_MARIO_JUMP_RUN_RIGHT				1192



#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_LEFT			1311
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT			1312

#define ID_ANI_MARIO_SMALL_WALKING_LEFT			1321
#define ID_ANI_MARIO_SMALL_WALKING_RIGHT		1322

#define ID_ANI_MARIO_SMALL_RUNNING_LEFT			1331
#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT		1332

#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT		1341
#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT		1342

#define ID_ANI_MARIO_SMALL_BRACE_LEFT			1361
#define ID_ANI_MARIO_SMALL_BRACE_RIGHT			1362

#define ID_ANI_MARIO_SMALL_KICK_LEFT			1371
#define ID_ANI_MARIO_SMALL_KICK_RIGHT			1372

#define ID_ANI_MARIO_SMALL_HOLD_LEFT			1381
#define ID_ANI_MARIO_SMALL_HOLD_RIGHT			1382

#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT		1391
#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT		1392



// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT			1511
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT			1512

#define ID_ANI_MARIO_RACCOON_WALKING_LEFT		1521
#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT		1522

#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT		1531
#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT		1532

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT		1541
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT	1542

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT		1591
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT		1592

#define ID_ANI_MARIO_RACCOON_SIT_LEFT			1551
#define ID_ANI_MARIO_RACCOON_SIT_RIGHT			1552

#define ID_ANI_MARIO_RACCOON_BRACE_LEFT			1561
#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT		1562

#define ID_ANI_MARIO_RACCOON_KICK_LEFT			1571
#define ID_ANI_MARIO_RACCOON_KICK_RIGHT			1572

#define ID_ANI_MARIO_RACCOON_HOLD_LEFT			1581
#define ID_ANI_MARIO_RACCOON_HOLD_RIGHT			1582

#define ID_ANI_MARIO_RACCOON_TAIL_WHIP_LEFT		1611
#define ID_ANI_MARIO_RACCOON_TAIL_WHIP_RIGHT	1612

#define ID_ANI_MARIO_RACCOON_TAIL_WAG_LEFT		1621
#define ID_ANI_MARIO_RACCOON_TAIL_WAG_RIGHT		1622

#pragma endregion

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


#pragma region MARIO_SIZE

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24

#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_RACCOON_BBOX_WIDTH  22
#define MARIO_RACCOON_BBOX_HEIGHT 24

#define MARIO_RACCOON_SITTING_BBOX_WIDTH  22
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#pragma endregion


#pragma region MARIO_TIME

#define MARIO_UNTOUCHABLE_TIME 2500


#define MARIO_MAX_TOTAL_FLY_TIME 4000

// When mario starts flying, the player has to keep hitting the "S" key to keep him in the sky
// or else, after this amount of time, mario will fall, but the player can hit "S" key to make him
// fly again, as long as the the total fly time hasn't been passed
#define MARIO_WAIT_BEFORE_FALLING_AFTER_FLY 200

#define MARIO_WAIT_BEFORE_FALLING_AFTER_TAIL_WAG 300

#define MARIO_RACCOON_TAIL_WHIP_ANI_TIMEOUT 200
#pragma endregion 


#define BLOCK_PUSH_FACTOR_GHOST_PLATFORM 1.0f

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isFlying;
	BOOLEAN isTrulyFalling;
	BOOLEAN isTailWhipping;

	BOOLEAN isKicking;
	BOOLEAN isReadyToHoldShell;
	BOOLEAN isHoldingShell;

	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int untouchable;
	ULONGLONG untouchable_start;

	// Mario can fly for 4 seconds in total
	ULONGLONG fly_total_start;

	// During the 4 seconds of total fly time, mario can fly a bit, then fall, then fly again, this
	// timer is for that case
	ULONGLONG fly_individual_start;

	ULONGLONG tail_wag_start;

	ULONGLONG tail_whip_start;


	BOOLEAN isOnPlatform;

	LPPMETER pMeter;

	CKoopaRedNormal* shellBeingHeld;

	// *** CONST *** pointer to the current playscene
	const LPPLAYSCENE currentScene;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithGoombaRedWing(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopaNormal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithItem(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBall(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickQuestionMark(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

public:
	CMario(float x, float y, const LPPLAYSCENE& currentScene)
		: CGameObject(x, y), currentScene(currentScene)
	{
		isSitting = false;
		isFlying = false;
		isTrulyFalling = false;
		isTailWhipping = false;
		isKicking = false;
		isReadyToHoldShell = false;
		isHoldingShell = false;

		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;

		fly_total_start = -1;
		fly_individual_start = -1;
		tail_wag_start = -1;

		tail_whip_start = -1;;

		isOnPlatform = false;

		pMeter = new CPMeter();
		shellBeingHeld = NULL;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable() { return (state != MARIO_STATE_DIE); }

	int IsBlocking() { return 0; }

	BOOLEAN IsFlying() { return isFlying; }

	BOOLEAN IsTrulyFalling() { return isTrulyFalling; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	float GetBBoxHeight();
	void SetLevel(int l);
	int GetLevel();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	float GetBBoxWidth();

	// This is so that mario will always be the first object in the vector "objects"
	virtual int GetRenderPriority() { return 99999999; }

	bool IsPMeterFullyCharged() { return pMeter->isFullyCharged(); }

	bool IsHoldingShell() { return isHoldingShell; }

	// This if for when koopa turns back to shell and mario no longer holds the shell
	void ReleaseHeldShell() { isHoldingShell = false; ; shellBeingHeld = NULL;};

	void KickHeldShell();
	//void NotifyPMeterAKeyHit() { pMeter->SetState(P_METER_STATE_KEY_A_HIT); }
};