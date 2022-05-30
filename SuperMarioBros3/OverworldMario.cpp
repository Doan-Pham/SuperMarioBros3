#include "OverworldMario.h"
#include "Animations.h"

#include "debug.h"

COverworldMario::COverworldMario(float x, float y) : CGameObject(x, y)
{
}

void COverworldMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void COverworldMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MARIO_OVERWORLD)->Render(x, y);
	DebugOutTitle(L"mario_x : %0.5f, mario-y : %0.5f",x, y);
}

void COverworldMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
