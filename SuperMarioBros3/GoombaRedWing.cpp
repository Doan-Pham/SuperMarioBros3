#include "GoombaRedWing.h"
#include "debug.h"

#include "DeadZone.h"
#include "PlatformGhost.h"
#include "SpecialEffectManager.h"

CGoombaRedWing::CGoombaRedWing(float x, float y) : CGameObject(x, y)
{
	this->ay = GOOMBA_RED_WING_GRAVITY;
	this->nx = -1;
	die_start = -1;
	landing_start = GetTickCount64();
	isOnPlatform = false;
	isStillHaveWing = true;

	SetState(GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING);
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
	//DebugOutTitle(L"x %0.5f, y %0.5f, vx %0.5f, vy %0.5f", x, y, vx, vy);
	vy += ay * dt;

	if ((state == GOOMBA_RED_WING_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) || 
		(state == GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT + GOOMBA_DIE_DELAY)))
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

	if (dynamic_cast<CDeadZone*>(e->obj))
		SetState(GOOMBA_RED_WING_STATE_DIE);

	else if (dynamic_cast<CPlatformGhost*>(e->obj))
		OnCollisionWithPlatformGhost(e);
}

void CGoombaRedWing::OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		isOnPlatform = true;

		CPlatformGhost* platform = dynamic_cast<CPlatformGhost*>(e->obj);
		float platform_l, platform_t, platform_r, platform_b;

		platform->GetBoundingBox(platform_l, platform_t, platform_r, platform_b);
		y = platform_t - GetBBoxHeight() / 2 - BLOCK_PUSH_FACTOR_GHOST_PLATFORM;

		vy = 0;
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

	else if (state == GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS)
		aniId = ID_ANI_GOOMBA_RED_WING_HIT_BY_DEADLY_ATTACKS;
	//aniId = ID_ANI_GOOMBA_RED_WING_WALKING_CLOSED_WING;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoombaRedWing::SetState(int state)
{
	if (this->state == GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS) return;
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
		isStillHaveWing = false;
		die_start = GetTickCount64();
		y += (GOOMBA_NORMAL_BBOX_HEIGHT - GOOMBA_DIE_BBOX_HEIGHT) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS:
	{
		die_start = GetTickCount64();
		vx = nx * GOOMBA_WALKING_SPEED;
		vy = -GOOMBA_RED_WING_DIE_BOUNCE_SPEED;
		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_SCORES_APPEAR, GOOMBA_RED_WING_SCORES_GIVEN_WHEN_HIT);
		break;
	}
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

float CGoombaRedWing::GetBBoxHeight()
{
	float cur_bbox_l, cur_bbox_t, cur_bbox_r, cur_bbox_b;
	GetBoundingBox(cur_bbox_l, cur_bbox_t, cur_bbox_r, cur_bbox_b);
	return cur_bbox_b - cur_bbox_t;
}