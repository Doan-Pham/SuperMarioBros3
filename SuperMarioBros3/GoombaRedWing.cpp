#include "GoombaRedWing.h"

CGoombaRedWing::CGoombaRedWing(float x, float y) : CGameObject(x, y)
{
}


void CGoombaRedWing::SetState(int state)
{
}

void CGoombaRedWing::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CGoombaRedWing::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CGoombaRedWing::OnNoCollision(DWORD dt)
{
}

void CGoombaRedWing::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CGoombaRedWing::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_GOOMBA_RED_WING_HOPPING)->Render(x, y);
}


