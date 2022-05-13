#include "FireBall.h"
#include "Goomba.h"
#include "GoombaRedWing.h"
#include "KoopaRedNormal.h"



CFireBall::CFireBall(float x, float y, int nx): CGameObject(x, y)
{
	this->nx = nx;
	vx = nx * FIREBALL_SPEED_X;
	vy = 0;
	ay = FIREBALL_GRAVITY;
	isDestroyed = false;
}

void CFireBall::Render()
{
	if (isDestroyed) return;
	CAnimations::GetInstance()->Get(ID_ANI_FIREBALL_MOVING)->Render(x, y);
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed) return;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CFireBall::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireBall::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = -FIREBALL_SPEED_Y;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
		isDestroyed = true;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
	{
		e->obj->SetState(GOOMBA_STATE_DIE);
		isDestroyed = true;
	}

	if (dynamic_cast<CGoombaRedWing*>(e->obj))
	{
		e->obj->SetState(GOOMBA_RED_WING_STATE_DIE);
		isDestroyed = true;
	}

	if (dynamic_cast<CKoopaRedNormal*>(e->obj) || dynamic_cast<CPlantRedFire*>(e->obj))
	{
		e->obj->Delete();
		isDestroyed = true;
	}

}
void CFireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIREBALL_BBOX_WIDTH / 2;
	t = y - FIREBALL_BBOX_HEIGHT / 2;
	r = l + FIREBALL_BBOX_WIDTH;
	b = t + FIREBALL_BBOX_HEIGHT;
}
