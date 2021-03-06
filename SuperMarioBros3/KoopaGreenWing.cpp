#include "KoopaGreenWing.h"

void CKoopaGreenWing::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CKoopa::Update(dt, coObjects);
}

int CKoopaGreenWing::GetAniId()
{
	if (state == KOOPA_STATE_WALKING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_GREEN_WING_WALKING_LEFT;
		else return ID_ANI_KOOPA_GREEN_WING_WALKING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_STILL_DOWNSIDE ||
		state == KOOPA_STATE_SHELL_MARIO_HOLD_DOWNSIDE)
	{
		if (GetTickCount64() - shell_start > KOOPA_SHELL_SHAKE_TIME)
			return ID_ANI_KOOPA_GREEN_WING_SHELL_SHAKE_DOWNSIDE;
		else
			return ID_ANI_KOOPA_GREEN_WING_SHELL_STILL_DOWNSIDE;
	}

	if (state == KOOPA_STATE_SHELL_STILL_UPSIDE ||
		state == KOOPA_STATE_SHELL_MARIO_HOLD_UPSIDE)
	{
		if (GetTickCount64() - shell_start > KOOPA_SHELL_SHAKE_TIME)
			return ID_ANI_KOOPA_GREEN_WING_SHELL_SHAKE_UPSIDE;
		else
			return ID_ANI_KOOPA_GREEN_WING_SHELL_STILL_UPSIDE;
	}
	if (state == KOOPA_STATE_DIE) return ID_ANI_KOOPA_GREEN_WING_SHELL_STILL_UPSIDE;

	if (state == KOOPA_STATE_SHELL_MOVING_DOWNSIDE)
		return ID_ANI_KOOPA_GREEN_WING_SHELL_MOVING_DOWNSIDE;

	if (state == KOOPA_STATE_SHELL_MOVING_UPSIDE)
		return ID_ANI_KOOPA_GREEN_WING_SHELL_MOVING_UPSIDE;

	if (state == KOOPA_STATE_HOPPING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_GREEN_WING_HOPPING_LEFT;
		else return ID_ANI_KOOPA_GREEN_WING_HOPPING_RIGHT;
	}
	return -1;
}

void CKoopaGreenWing::Render()
{
	int aniId = GetAniId();
	if (aniId == -1)
	{
		DebugOut(L"[ERROR] Render failed ! Can't find koopa_green_wing's ani \n");
		return;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();

	//DebugOutTitle(L"koopa's state: %d", state);
	//DebugOut(L"[INFO] Koopa's Render() has been called \n");
}
