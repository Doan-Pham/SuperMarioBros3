#include "FireShot.h"

CFireShot::CFireShot(float x, float y, float destination_x, float destination_y)
	: CGameObject(x, y)
{
	this->destination_x = destination_x;
	this->destination_y = destination_y;

	// This prevents the case where destination_x == x (or des_y == y) which leads to division by 0 error
	this->nx = (destination_x - x ) == 0 ? 0 : (int)(destination_x - x) / (int) abs(destination_x - x);
	this->ny = (destination_y - y ) == 0 ? 0 : (int)(destination_y - y) / (int) abs(destination_y - y);

	// This vx adjustment keeps the time fireshot takes to reach mario the same
	// TODO: This is not how it goes in the original game, however it's because in our framework, vx vy
	// presents both velocity and direction
	vx = nx * abs(destination_x - x) / FIRESHOT_STANDARD_TIME_X;

	// vy is adjusted based on vx to create the desired shooting angle
	vy = ny * abs(vx) * abs((destination_y - y) / (destination_x - x));
}

void CFireShot::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FIREBALL_MOVING)->Render(x, y);
}

void CFireShot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	if ((x < cam_x) || (x > (cam_x + SCREEN_WIDTH)) ||
		(y < cam_y) || (y > (cam_y + SCREEN_HEIGHT)))
		this->Delete();
}

void CFireShot::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIRESHOT_BBOX_WIDTH / 2;
	t = y - FIRESHOT_BBOX_HEIGHT / 2;
	r = l + FIRESHOT_BBOX_WIDTH;
	b = t + FIRESHOT_BBOX_HEIGHT;
}
