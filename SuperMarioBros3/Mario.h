#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "AttackBBox.h"

#include "PMeter.h"
#include "FireBall.h"
#include "Hammer.h"
#include "debug.h"
#include "Koopa.h"
#include "Pipe.h"

class CPlayScene;
typedef CPlayScene* LPPLAYSCENE;

class CKoopa;

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

#define MARIO_STATE_THROW_FIRE		1100
#define MARIO_STATE_THROW_HAMMER	1200

#define MARIO_STATE_COURSE_CLEAR		1300
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


// FIRE MARIO

#define ID_ANI_MARIO_FIRE_IDLE_LEFT					1711
#define ID_ANI_MARIO_FIRE_IDLE_RIGHT				1712

#define ID_ANI_MARIO_FIRE_WALKING_LEFT				1721
#define ID_ANI_MARIO_FIRE_WALKING_RIGHT				1722

#define ID_ANI_MARIO_FIRE_RUNNING_LEFT				1731
#define ID_ANI_MARIO_FIRE_RUNNING_RIGHT				1732

#define ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT			1741
#define ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT			1742

#define ID_ANI_MARIO_FIRE_SIT_LEFT					1751
#define ID_ANI_MARIO_FIRE_SIT_RIGHT					1752

#define ID_ANI_MARIO_FIRE_BRACE_LEFT				1761
#define ID_ANI_MARIO_FIRE_BRACE_RIGHT				1762

#define ID_ANI_MARIO_FIRE_KICK_LEFT					1771
#define ID_ANI_MARIO_FIRE_KICK_RIGHT				1772

#define ID_ANI_MARIO_FIRE_HOLD_LEFT					1781
#define ID_ANI_MARIO_FIRE_HOLD_RIGHT				1782

#define ID_ANI_MARIO_FIRE_JUMP_RUN_LEFT				1791
#define ID_ANI_MARIO_FIRE_JUMP_RUN_RIGHT			1792

#define ID_ANI_MARIO_FIRE_THROW_FIREBALL_LEFT		1811
#define ID_ANI_MARIO_FIRE_THROW_FIREBALL_RIGHT		1812

// HAMMER MARIO

#define ID_ANI_MARIO_HAMMER_IDLE_LEFT				1911
#define ID_ANI_MARIO_HAMMER_IDLE_RIGHT				1912

#define ID_ANI_MARIO_HAMMER_WALKING_LEFT			1921
#define ID_ANI_MARIO_HAMMER_WALKING_RIGHT			1922

#define ID_ANI_MARIO_HAMMER_RUNNING_LEFT			1931
#define ID_ANI_MARIO_HAMMER_RUNNING_RIGHT			1932

#define ID_ANI_MARIO_HAMMER_JUMP_WALK_LEFT			1941
#define ID_ANI_MARIO_HAMMER_JUMP_WALK_RIGHT			1942

#define ID_ANI_MARIO_HAMMER_SIT_LEFT				1951
#define ID_ANI_MARIO_HAMMER_SIT_RIGHT				1952

#define ID_ANI_MARIO_HAMMER_BRACE_LEFT				1961
#define ID_ANI_MARIO_HAMMER_BRACE_RIGHT				1962

#define ID_ANI_MARIO_HAMMER_KICK_LEFT				1971
#define ID_ANI_MARIO_HAMMER_KICK_RIGHT				1972

#define ID_ANI_MARIO_HAMMER_HOLD_LEFT				1981
#define ID_ANI_MARIO_HAMMER_HOLD_RIGHT				1982

#define ID_ANI_MARIO_HAMMER_JUMP_RUN_LEFT			1991
#define ID_ANI_MARIO_HAMMER_JUMP_RUN_RIGHT			1992

#define ID_ANI_MARIO_HAMMER_THROW_HAMMER_LEFT		2011
#define ID_ANI_MARIO_HAMMER_THROW_HAMMER_RIGHT		2012

#pragma endregion


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON 3
#define MARIO_LEVEL_FIRE	4
#define MARIO_LEVEL_HAMMER	5

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

#define MARIO_FIRE_THROW_FIREBALL_ANI_TIMEOUT 200
#pragma endregion 

#define MARIO_FIRE_MAX_FIREBALLS_NUM	2
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

	BOOLEAN isThrowingFireball;
	BOOLEAN isThrowingHammer;

	// This vector manages how many fireballs mario can throw, when and how the fireballs are deleted
	vector<CFireBall*> fireBalls;
	vector<CHammer*> hammers;
	CAttackBBox* raccoon_tail;

	CPipe* spawnPipeLocation;
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

	ULONGLONG throw_fireball_start;
	ULONGLONG throw_hammer_start;

	BOOLEAN isOnPlatform;

	LPPMETER pMeter;

	CKoopa* shellBeingHeld;

	// *** CONST *** pointer to the current playscene
	const LPPLAYSCENE currentScene;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithGoombaRedWing(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithItem(LPCOLLISIONEVENT e);
	void OnCollisionWithFireShot(LPCOLLISIONEVENT e);
	void OnCollisionWithDeadZone(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickQuestionMark(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickGlass(LPCOLLISIONEVENT e);
	void OnCollisionWithPBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();
	int GetAniIdFire();
	int GetAniIdHammer();

public:
	CMario(float x, float y, const LPPLAYSCENE& currentScene);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Render();
	void SetState(int state);

	int IsCollidable() { return (state != MARIO_STATE_DIE); }

	int IsBlocking() { return 0; }

	BOOLEAN IsFlying() { return isFlying; }

	BOOLEAN IsTrulyFalling() { return isTrulyFalling; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	float GetBBoxWidth();
	float GetBBoxHeight();

	// This is so that mario will always be the first object in the vector "objects"
	virtual int GetRenderPriority() { return 99999999; }

	bool IsPMeterFullyCharged() { return pMeter->isFullyCharged(); }

	bool IsHoldingShell() { return isHoldingShell; }

	// This if for when koopa turns back to shell and mario no longer holds the shell
	void ReleaseHeldShell() { isHoldingShell = false; ; shellBeingHeld = NULL;};

	void KickHeldShell();

	void SetSpawnPipeLocation(CPipe* pipe) { this->spawnPipeLocation = pipe; }
	//void NotifyPMeterAKeyHit() { pMeter->SetState(P_METER_STATE_KEY_A_HIT); }
};