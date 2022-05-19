#pragma once

#include "GameObject.h"
#include "debug.h"
#include "AttachedBBox.h"
#include "PlayScene.h"

#define ID_ANI_KOOPA_WALKING_LEFT					5231
#define ID_ANI_KOOPA_WALKING_RIGHT					5232

#define ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_LEFT		5233
#define ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_RIGHT		5234
#define ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_LEFT		5235
#define ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_RIGHT	5236

#define ID_ANI_KOOPA_SHELL_UPSIDE_STILL				5237
#define ID_ANI_KOOPA_SHELL_DOWNSIDE_STILL			5238

#define KOOPA_NORMAL_BBOX_WIDTH		16
#define KOOPA_NORMAL_BBOX_HEIGHT	26

#define KOOPA_SHELL_BBOX_WIDTH		16
#define KOOPA_SHELL_BBOX_HEIGHT		16

#define KOOPA_SCORES_GIVEN_WHEN_HIT 100


#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_SHELL_MOVING_SPEED	0.15f

#define KOOPA_STATE_WALKING								100

#define KOOPA_STATE_SHELL_DOWNSIDE_STILL				200
#define KOOPA_STATE_SHELL_DOWNSIDE_MARIO_HOLD			300
#define KOOPA_STATE_SHELL_DOWNSIDE_MOVING				400

#define KOOPA_STATE_SHELL_UPSIDE_STILL					500
#define KOOPA_STATE_SHELL_UPSIDE_MARIO_HOLD				600
#define KOOPA_STATE_SHELL_UPSIDE_MOVING					700

#define KOOPA_STATE_DIE									800

#define KOOPA_SHELL_TIMEOUT	9000
#define BLOCK_PUSH_FACTOR_GHOST_PLATFORM 1.0f

class CPlayScene;
typedef CPlayScene* LPPLAYSCENE;

class CKoopaRedNormal : public CGameObject
{
protected:

	float ay;

	CAttachedBBox* attachedBBox;

	BOOLEAN isShell;
	BOOLEAN isBeingHeld;

	ULONGLONG shell_start;
	// *** CONST *** pointer to the current playscene
	const LPPLAYSCENE currentScene;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int GetAniId();
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e);

	void OnCollisionWithPlantRedFire(LPCOLLISIONEVENT e);

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

	void OnCollisionWithBrickQuestionMark(LPCOLLISIONEVENT e);


public:
	CKoopaRedNormal(float x, float y, const LPPLAYSCENE& currentScene);
	void SetDirection(int nx) { this->nx = nx; }
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return KOOPA_SCORES_GIVEN_WHEN_HIT; }
};

