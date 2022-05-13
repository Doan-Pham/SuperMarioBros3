#include "GoombaRedWing.h"
#include "debug.h"

CGoombaRedWing::CGoombaRedWing(float x, float y) : CGameObject(x, y)
{
	this->ay = GOOMBA_GRAVITY;
	this->nx = -1;
	die_start = -1;
	landing_start = GetTickCount64();
	isOnPlatform = false;
	isStillHaveWing = true;

	SetState(GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING);
}


void CGoombaRedWing::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_RED_WING_STATE_WALKING_NORMAL:
		vx = nx * GOOMBA_WALKING_SPEED;
		isStillHaveWing = false;
		break;

	case GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING:
		vx = nx * GOOMBA_WALKING_SPEED;
		break;

	case GOOMBA_RED_WING_STATE_HOPPING:
		if (isOnPlatform) 
		{
			y -= (GOOMBA_OPEN_WING_BBOX_HEIGHT - GOOMBA_CLOSED_WING_BBOX_HEIGHT);
			vy = -GOOMBA_HOPPING_SPEED;
		}
		break;

	case GOOMBA_RED_WING_STATE_JUMPING:
		if (isOnPlatform)
		{
			y -= (GOOMBA_OPEN_WING_BBOX_HEIGHT - GOOMBA_CLOSED_WING_BBOX_HEIGHT);
			vy = -GOOMBA_JUMPING_SPEED;
		}
		break;

	case GOOMBA_RED_WING_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_NORMAL_BBOX_HEIGHT - GOOMBA_DIE_BBOX_HEIGHT) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	}
}

void CGoombaRedWing::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_RED_WING_STATE_DIE)
	{
		left = x - GOOMBA_NORMAL_BBOX_WIDTH / 2;
		top = y - GOOMBA_DIE_BBOX_HEIGHT / 2;
		right = left + GOOMBA_NORMAL_BBOX_WIDTH;
		bottom = top + GOOMBA_DIE_BBOX_HEIGHT;
	}
	else if (state == GOOMBA_RED_WING_STATE_WALKING_NORMAL)
	{
		left = x - GOOMBA_NORMAL_BBOX_WIDTH / 2;
		top = y - GOOMBA_NORMAL_BBOX_HEIGHT / 2;
		right = left + GOOMBA_NORMAL_BBOX_WIDTH;
		bottom = top + GOOMBA_NORMAL_BBOX_HEIGHT;
	}
	else if (state == GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING)
	{
		left = x - GOOMBA_CLOSED_WING_BBOX_WIDTH / 2;
		top = y - GOOMBA_CLOSED_WING_BBOX_HEIGHT / 2;
		right = left + GOOMBA_CLOSED_WING_BBOX_WIDTH;
		bottom = top + GOOMBA_CLOSED_WING_BBOX_HEIGHT;
	}
	else if (state == GOOMBA_RED_WING_STATE_HOPPING || state == GOOMBA_RED_WING_STATE_JUMPING)
	{
		left = x - GOOMBA_OPEN_WING_BBOX_WIDTH / 2;
		top = y - GOOMBA_OPEN_WING_BBOX_HEIGHT / 2;
		right = left + GOOMBA_OPEN_WING_BBOX_WIDTH;
		bottom = top + GOOMBA_OPEN_WING_BBOX_HEIGHT;
	}
}

void CGoombaRedWing::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ULONGLONG now = GetTickCount64();
	if (isOnPlatform && isStillHaveWing)
	{
		if (now - landing_start > GOOMBA_WALK_CLOSED_WING_TIMEOUT)
		{
			if (now - landing_start - GOOMBA_WALK_CLOSED_WING_TIMEOUT < GOOMBA_HOP_TIMEOUT)
				SetState(GOOMBA_RED_WING_STATE_HOPPING);
			else
			{
				SetState(GOOMBA_RED_WING_STATE_JUMPING);;
				landing_start = GetTickCount64();
			}
				
		}
		else SetState(GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING);
	}
	//DebugOutTitle(L"land_start: %I64u, now - land_start: %I64u", landing_start, now - landing_start);
	DebugOutTitle(L"x %0.5f, y %0.5f, vx %0.5f, vy %0.5f", x, y, vx, vy);
	vy += ay * dt;

	if ((state == GOOMBA_RED_WING_STATE_DIE) && 
		(GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	isOnPlatform = false;
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
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		if (e->ny < 0) isOnPlatform = true;
		vy = 0;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
		nx = -nx;
	}
}

void CGoombaRedWing::Render()
{
	int aniId = ID_ANI_GOOMBA_RED_WING_WALKING_CLOSED_WING;

	if (state == GOOMBA_RED_WING_STATE_WALKING_NORMAL)
		aniId = ID_ANI_GOOMBA_RED_WING_WALKING_NORMAL;

	else if (state == GOOMBA_RED_WING_STATE_HOPPING)
		aniId = ID_ANI_GOOMBA_RED_WING_HOPPING;

	else if (state == GOOMBA_RED_WING_STATE_JUMPING)
		aniId = ID_ANI_GOOMBA_RED_WING_JUMPING;

	else if (state == GOOMBA_RED_WING_STATE_DIE)
		aniId = ID_ANI_GOOMBA_RED_WING_DIE;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}


