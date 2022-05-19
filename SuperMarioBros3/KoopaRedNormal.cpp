#include "KoopaRedNormal.h"

#include "Mario.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "PlantRedFire.h"

#include "BrickQuestionMark.h"
#include "PlatformGhost.h"
#include "DeadZone.h"

#include "MushroomBig.h"
#include "Leaf.h"

CKoopaRedNormal::CKoopaRedNormal(float x, float y, const LPPLAYSCENE& currentScene)
	:CKoopa(x, y, currentScene)
{
}


void CKoopaRedNormal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_WALKING &&
		attachedBBox != NULL &&
		attachedBBox->GetState() == ATTACHED_BBOX_STATE_FALL)
	{
		vx = -vx;
		nx = -nx;
		attachedBBox->Delete();
		attachedBBox = new CAttachedBBox(x + nx * KOOPA_NORMAL_BBOX_WIDTH, y, vx, vy);
		this->currentScene->AddObject(attachedBBox);
	}

	vy += ay * dt;


	if (isShell && GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT &&
		state != KOOPA_STATE_SHELL_DOWNSIDE_MOVING &&
		state != KOOPA_STATE_SHELL_UPSIDE_MOVING)
	{
		if (isBeingHeld)
		{
			CMario* mario = (CMario*)this->currentScene->GetPlayer();
			mario->ReleaseHeldShell();
			isBeingHeld = false;
		}
		SetState(KOOPA_STATE_WALKING);
	}
			
	CKoopa::Update(dt, coObjects);

	//if (isBeingHeld)
	//DebugOut(L"[INFO] Koopa x : %0.5f, koopa y : %0.5f, vx: %0.5f, vy :%0.5f \n", x, y, vx, vy);
}

int CKoopaRedNormal::GetAniId()
{
	if (state == KOOPA_STATE_WALKING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_RED_NORMAL_WALKING_LEFT;
		else return ID_ANI_KOOPA_RED_NORMAL_WALKING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_DOWNSIDE_STILL ||
		state == KOOPA_STATE_SHELL_DOWNSIDE_MARIO_HOLD)
		return ID_ANI_KOOPA_RED_NORMAL_SHELL_DOWNSIDE_STILL;

	if (state == KOOPA_STATE_SHELL_UPSIDE_STILL ||
		state == KOOPA_STATE_SHELL_UPSIDE_MARIO_HOLD)
		return ID_ANI_KOOPA_RED_NORMAL_SHELL_UPSIDE_STILL;

	if (state == KOOPA_STATE_SHELL_DOWNSIDE_MOVING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_RED_NORMAL_SHELL_DOWNSIDE_MOVING_LEFT;
		else return ID_ANI_KOOPA_RED_NORMAL_SHELL_DOWNSIDE_MOVING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_UPSIDE_MOVING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_RED_NORMAL_SHELL_UPSIDE_MOVING_LEFT;
		else return ID_ANI_KOOPA_RED_NORMAL_SHELL_UPSIDE_MOVING_RIGHT;
	}

	return -1;
}

void CKoopaRedNormal::Render()
{
	int aniId = GetAniId();
	if (aniId == -1)
	{
		DebugOut(L"[ERROR] Render failed ! Can't find koopa_red_normal's ani \n");
		return;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();

	//DebugOutTitle(L"koopa's state: %d", state);
	//DebugOut(L"[INFO] Koopa's Render() has been called \n");
}

