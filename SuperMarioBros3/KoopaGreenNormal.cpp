#include "KoopaGreenNormal.h"

#include "Mario.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "PlantRedFire.h"

#include "BrickQuestionMark.h"
#include "PlatformGhost.h"
#include "DeadZone.h"

#include "MushroomBig.h"
#include "Leaf.h"

void CKoopaGreenNormal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CKoopa::Update(dt, coObjects);

	//if (isBeingHeld)
	//DebugOut(L"[INFO] Koopa x : %0.5f, koopa y : %0.5f, vx: %0.5f, vy :%0.5f \n", x, y, vx, vy);
}

int CKoopaGreenNormal::GetAniId()
{
	if (state == KOOPA_STATE_WALKING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_GREEN_NORMAL_WALKING_LEFT;
		else return ID_ANI_KOOPA_GREEN_NORMAL_WALKING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_STILL_DOWNSIDE ||
		state == KOOPA_STATE_SHELL_MARIO_HOLD_DOWNSIDE)
		return ID_ANI_KOOPA_GREEN_NORMAL_SHELL_STILL_DOWNSIDE;

	if (state == KOOPA_STATE_SHELL_STILL_UPSIDE ||
		state == KOOPA_STATE_SHELL_MARIO_HOLD_UPSIDE ||
		state == KOOPA_STATE_DIE)
		return ID_ANI_KOOPA_GREEN_NORMAL_SHELL_STILL_UPSIDE;

	if (state == KOOPA_STATE_SHELL_MOVING_DOWNSIDE)
		return ID_ANI_KOOPA_GREEN_NORMAL_SHELL_MOVING_DOWNSIDE;

	if (state == KOOPA_STATE_SHELL_MOVING_UPSIDE)
		return ID_ANI_KOOPA_GREEN_NORMAL_SHELL_MOVING_UPSIDE;

	return -1;
}

void CKoopaGreenNormal::Render()
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

