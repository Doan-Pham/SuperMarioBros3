#include "GoombaRedWing.h"

#include "Goomba.h"
#include "Mario.h"

CGoombaRedWing::CGoombaRedWing(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_RED_WING_STATE_WALKING_NORMAL);
}


void CGoombaRedWing::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_RED_WING_STATE_WALKING_NORMAL:
		vx = -GOOMBA_WALKING_SPEED;
		break;

	case GOOMBA_RED_WING_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_DIE_BBOX_HEIGHT) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	}
}

void CGoombaRedWing::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoombaRedWing::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_RED_WING_STATE_DIE) && 
		(GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGoombaRedWing::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CGoombaRedWing::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (dynamic_cast<CMario*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoombaRedWing::Render()
{
	int aniId = ID_ANI_GOOMBA_RED_WING_WALKING;
	if (state == GOOMBA_RED_WING_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_RED_WING_DIE;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}


