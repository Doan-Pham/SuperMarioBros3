#include "MushroomBig.h"
CMushroomBig::CMushroomBig(float x, float y) : CItem(x, y)
{
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	vx = -MUSHROOM_MOVING_SPEED;
	//die_start = -1;
	//SetState(MUSHROOM_STATE_WALKING);
}
void CMushroomBig::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM_MOVING)->Render(x, y);
}

void CMushroomBig::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroomBig::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushroomBig::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroomBig::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}


