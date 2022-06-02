#include "MushroomBig.h"
#include "PlatformGhost.h"

CMushroomBig::CMushroomBig(float x, float y) : CItem(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->y_destination = y - MUSHROOM_BBOX_HEIGHT;
	SetState(MUSHROOM_STATE_HIDING);
}

void CMushroomBig::Render()
{
	if (state != MUSHROOM_STATE_HIDING)
		CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM_BIG_MOVING)->Render(x, y);
}

void CMushroomBig::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MUSHROOM_STATE_HIDING) return;
	vy += ay * dt;
	vx += ax * dt;

	// Make mushroom stop at a certain position so it can start processing collisions with its
	// container brick
	if (y + vy * dt < y_destination)
	{
		vy = (y_destination - y) / dt;
	}

	//DebugOutTitle(L"x : %0.5f, y :%0.5f, vx :%0.5f, vy :%0.5f", x, y, vx, vy);
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
	if (y == y_destination) SetState(MUSHROOM_STATE_MOVING);;
}

void CMushroomBig::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//TODO: Need a more general cast than this, because there will many more enemies
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
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

	if (dynamic_cast<CPlatformGhost*>(e->obj))
		OnCollisionWithPlatformGhost(e);
}
void CMushroomBig::OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		CPlatformGhost* platform = dynamic_cast<CPlatformGhost*>(e->obj);
		float platform_l, platform_t, platform_r, platform_b;
		platform->GetBoundingBox(platform_l, platform_t, platform_r, platform_b);

		float mushroom_l, mushroom_t, mushroom_r, mushroom_b;
		this->GetBoundingBox(mushroom_l, mushroom_t, mushroom_r, mushroom_b);

		// Have to directly change mushroom's y because ghost platform is non-blocking, so the 
		// collisions with it are not handled by the framework (inside that, the coordinates
		// of the src_obj that collided with blocking things are automatically adjusted
		// to be the correct numbers using some calculations with BLOCK_PUSH_FACTOR)
		y = platform_t - (mushroom_b - mushroom_t) / 2 - BLOCK_PUSH_FACTOR_GHOST_PLATFORM;

		vy = 0;
	}
}

void CMushroomBig::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_HIDING:
		vy = 0;
		vx = 0;
		ay = 0;
		break;

	case MUSHROOM_STATE_APPEARING:
		vy = -MUSHROOM_APPEARING_SPEED;
		break;

	case MUSHROOM_STATE_MOVING:
		// If mario hits the brick more on the left side, mushroom moves to the right
		// and vice versa
		vx = -nx * MUSHROOM_MOVING_SPEED;
		ay = MUSHROOM_GRAVITY;
		break;
	}
}

