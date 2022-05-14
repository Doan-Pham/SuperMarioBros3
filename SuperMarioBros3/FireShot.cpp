#include "FireShot.h"

CFireShot::CFireShot(float x, float y, int destination_x, int destination_y)
	: CGameObject(x, y)
{
	this->destination_x = destination_x;
	this->destination_y = destination_y;
	this->nx = (destination_x - x ) / abs(destination_x - x);
	this->ny = (destination_y - y) / abs(destination_y - y);


	vx = nx * FIRESHOT_SPEED_X ;
	vy = ny * abs(vx) * abs((destination_y - y)/(destination_x - x));
}

void CFireShot::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FIREBALL_MOVING)->Render(x, y);
}

void CFireShot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireShot::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIRESHOT_BBOX_WIDTH / 2;
	t = y - FIRESHOT_BBOX_HEIGHT / 2;
	r = l + FIRESHOT_BBOX_WIDTH;
	b = t + FIRESHOT_BBOX_HEIGHT;
}
