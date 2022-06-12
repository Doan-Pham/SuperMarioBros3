#include "Goomba.h"
#include "Mario.h"
#include "DeadZone.h"
#include "SpecialEffectManager.h"

CGoomba::CGoomba(float x, float y):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}


void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE  && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT)) || 
		(state == GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT + GOOMBA_DIE_DELAY)))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 
	if (dynamic_cast<CMario*>(e->obj)) return;

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

	if (dynamic_cast<CDeadZone*>(e->obj))
		SetState(GOOMBA_STATE_DIE);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
		aniId = ID_ANI_GOOMBA_DIE;
	else if (state == GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS)
		aniId = ID_ANI_GOOMBA_HIT_BY_DEADLY_ATTACKS;
	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;

	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS:
		die_start = GetTickCount64();
		vx = nx * GOOMBA_WALKING_SPEED;
		vy = -GOOMBA_DIE_BOUNCE_SPEED;
		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_SCORES_APPEAR, GOOMBA_SCORES_GIVEN_WHEN_HIT);
		break;
	}

	DebugOutTitle(L"goomba_vx : %0.5f, goomba_nx : %i", vx, nx);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

