#include "Leaf.h"

CLeaf::CLeaf(float x, float y) : CItem(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->y_destination = y - LEAF_BOUNCE_DISTANCE;
	this->x_start = x;
	this->x_end = x + LEAF_MAX_X_DISTANCE;
	SetState(LEAF_STATE_HIDING);
}
void CLeaf::Render()
{
	if (state != LEAF_STATE_HIDING)
		CSprites::GetInstance()->Get(ID_SPRITE_LEAF_MOVING_LEFT)->Draw(x, y);
	RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == LEAF_STATE_HIDING) return;
	vy += ay * dt;
	vx += ax * dt;

	if (y + vy * dt < y_destination)
	{
		vy = (y_destination - y) / dt;
	}
	y += vy * dt;

	if (x + vx * dt >= x_end || x + vx * dt <= x_start)
	{
		vx = -vx;
	}
	x += vx * dt;

	if (y == y_destination) SetState(LEAF_STATE_MOVING);

	//DebugOutTitle(L"x : %0.5f, y :%0.5f, vx :%0.5f, vy :%0.5f", x, y, vx, vy);
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_HIDING:
		vy = 0;
		vx = 0;
		ay = 0;
		break;

	case LEAF_STATE_APPEARING:
		vy = -LEAF_APPEARING_SPEED;
		break;

	case LEAF_STATE_MOVING:
		vy = 0.05f;
		vx = LEAF_MOVING_SPEED;
		break;
	}
}

