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

	if (state == KOOPA_STATE_SHELL_STILL_DOWNSIDE ||
		state == KOOPA_STATE_SHELL_MARIO_HOLD_DOWNSIDE)
	{
		if (GetTickCount64() - shell_start > KOOPA_SHELL_SHAKE_TIME)
			return ID_ANI_KOOPA_RED_NORMAL_SHELL_SHAKE_DOWNSIDE;
		else 
			return ID_ANI_KOOPA_RED_NORMAL_SHELL_STILL_DOWNSIDE;
	}
		

	if (state == KOOPA_STATE_SHELL_STILL_UPSIDE ||
		state == KOOPA_STATE_SHELL_MARIO_HOLD_UPSIDE)
	{
		if (GetTickCount64() - shell_start > KOOPA_SHELL_SHAKE_TIME)
			return ID_ANI_KOOPA_RED_NORMAL_SHELL_SHAKE_UPSIDE;
		else
			return ID_ANI_KOOPA_RED_NORMAL_SHELL_STILL_UPSIDE;
	}
	if (state == KOOPA_STATE_DIE) return ID_ANI_KOOPA_RED_NORMAL_SHELL_STILL_UPSIDE;

	if (state == KOOPA_STATE_SHELL_MOVING_DOWNSIDE)
		return ID_ANI_KOOPA_RED_NORMAL_SHELL_MOVING_DOWNSIDE;

	if (state == KOOPA_STATE_SHELL_MOVING_UPSIDE)
		return ID_ANI_KOOPA_RED_NORMAL_SHELL_MOVING_UPSIDE;

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

