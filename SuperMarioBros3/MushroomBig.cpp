#include "MushroomBig.h"
CMushroomBig::CMushroomBig(float x, float y) : CItem(x, y)
{
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	isHidden = true;

	SetState(MUSHROOM_STATE_HIDING);
}
void CMushroomBig::Render()
{
	if (!isHidden)
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
void CMushroomBig::SetState(int state, int nx)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_HIDING:
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case MUSHROOM_STATE_APPEARING:
		vy = -MUSHROOM_APPEARING_SPEED;
		break;

	//If 
	case MUSHROOM_STATE_MOVING:
		// If mario hits the brick on the left side, mushroom moves to the right, and vice versa
		vx =  -nx * MUSHROOM_MOVING_SPEED;
		break;
	}
}

