#include "Leaf.h"

CLeaf::CLeaf(float x, float y) : CItem(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->y_bounce_destination = y - LEAF_BOUNCE_TRAVEL_DISTANCE;
	SetState(LEAF_STATE_HIDING);
}

void CLeaf::Render()
{
	if (state != LEAF_STATE_HIDING)
	{
		if (vx <= 0) CSprites::GetInstance()->Get(ID_SPRITE_LEAF_MOVING_LEFT)->Draw(x, y);
		else CSprites::GetInstance()->Get(ID_SPRITE_LEAF_MOVING_RIGHT)->Draw(x, y);
	}

	//RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == LEAF_STATE_HIDING) return;

	vy += ay * dt;
	vx += ax * dt;

	// Make leaf's vx change until it is equal to max_moving_speed, then change direction
	if (abs(vx) > LEAF_MAX_MOVING_SPEED)
	{
		ax = -ax;
		if (vx < 0) vx = -LEAF_MAX_MOVING_SPEED;
		else vx = LEAF_MAX_MOVING_SPEED;
	}

	// Adjust leaf's vy so it doesn't bounce past the destination
	if (y + vy * dt < y_bounce_destination)
	{
		vy = (y_bounce_destination - y) / dt;
	}
	y += vy * dt;
	if (y == y_bounce_destination) SetState(LEAF_STATE_MOVING);

	x += vx * dt;

	//DebugOutTitle(L"x : %0.5f, y :%0.5f, vx :%0.5f, vy :%0.5f, ax: %0.5f, x_start: %0.5f, x_end: %0.5f", x, y, vx, vy, ax);
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
		vy = LEAF_GRAVITY;
		ax = LEAF_MOVING_ACCELERATE;
		break;
	}
}

