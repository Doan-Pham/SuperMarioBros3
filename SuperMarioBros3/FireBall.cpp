#include "FireBall.h"

CFireBall::CFireBall(float x, float y, int destination_x, int destination_y)
	: CGameObject(x, y)
{
	this->destination_x = destination_x;
	this->destination_y = destination_y;
	this->nx = (destination_x - x ) / abs(destination_x - x);
	this->ny = (destination_y - y) / abs(destination_y - y);


	vx = nx * FIREBALL_SPEED_X ;
	vy = ny * abs(vx) * abs((destination_y - y)/(destination_x - x));
}

void CFireBall::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FIREBALL_MOVING)->Render(x, y);
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIREBALL_BBOX_WIDTH / 2;
	t = y - FIREBALL_BBOX_HEIGHT / 2;
	r = l + FIREBALL_BBOX_WIDTH;
	b = t + FIREBALL_BBOX_HEIGHT;
}
