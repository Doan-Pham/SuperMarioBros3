#include "AttachedBBox.h"
#include "Mario.h"
#include "PlatformGhost.h"

CAttachedBBox::CAttachedBBox(float x, float y, float vx, float vy) :CGameObject(x, y)
{
	this->ay = ATTACHED_BBOX__NORMAL_GRAVITY;
	this->vx = vx;
	this->vy = vy;
	this->y_original = y;
	SetState(ATTACHED_BBOX_STATE_MOVING);
}


void CAttachedBBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - ATTACHED_BBOX_BBOX_WIDTH / 2;
	top = y - ATTACHED_BBOX_BBOX_HEIGHT / 2;
	right = left + ATTACHED_BBOX_BBOX_WIDTH;
	bottom = top + ATTACHED_BBOX_BBOX_HEIGHT;
}


void CAttachedBBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (y > y_original + ATTACHED_BBOX_BBOX_HEIGHT)
	{
		SetState(ATTACHED_BBOX_STATE_FALL);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	DebugOutTitle(L"attached bbox: x %0.5f , y %0.5f, vx %0.5f, vy %0.5f", x, y, vx, vy);
}


void CAttachedBBox::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CAttachedBBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CAttachedBBox*>(e->obj)) return;
	if (dynamic_cast<CMario*>(e->obj)) return;

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
void CAttachedBBox::OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{

		CPlatformGhost* platform = dynamic_cast<CPlatformGhost*>(e->obj);
		float platform_l, platform_t, platform_r, platform_b;
		platform->GetBoundingBox(platform_l, platform_t, platform_r, platform_b);

		float koopa_l, koopa_t, koopa_r, koopa_b;
		this->GetBoundingBox(koopa_l, koopa_t, koopa_r, koopa_b);

		// Have to directly change koopa's y because ghost platform is non-blocking, so the 
		// collisions with it are not handled by the framework (inside that, the coordinates
		// of the src_obj that collided with blocking things are automatically adjusted
		// to be the correct numbers using some calculations with BLOCK_PUSH_FACTOR)
		y = platform_t - (koopa_b - koopa_t) / 2 - BLOCK_PUSH_FACTOR_GHOST_PLATFORM;

		vy = 0;
	}
}


void CAttachedBBox::Render()
{

	RenderBoundingBox();

	//DebugOut(L"[INFO] Koopa's Render() has been called \n");
}

void CAttachedBBox::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ATTACHED_BBOX_STATE_MOVING:
	{
		//vx = -0.2f;
		break;
	}
	}
}
