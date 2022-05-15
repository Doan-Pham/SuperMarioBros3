#include "Hammer.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "KoopaRedNormal.h"
#include "PlantRedFire.h"


CHammer::CHammer(float x, float y, int nx) : CGameObject(x, y)
{
	this->nx = nx;
	vx = nx * HAMMER_SPEED_X;
	vy = -HAMMER_SPEED_Y;
	ay = HAMMER_GRAVITY;
	isCollidable = true;
}

void CHammer::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_HAMMER_MOVING)->Render(x, y);
}

void CHammer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CHammer::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CHammer::OnCollisionWith(LPCOLLISIONEVENT e)
{
	isCollidable = true;

	if (dynamic_cast<CGoomba*>(e->obj))
		e->obj->SetState(GOOMBA_STATE_DIE);

	if (dynamic_cast<CGoombaRedWing*>(e->obj))
		e->obj->SetState(GOOMBA_RED_WING_STATE_DIE);

	if (dynamic_cast<CKoopaRedNormal*>(e->obj))
		e->obj->SetState(KOOPA_STATE_DIE);

	if (dynamic_cast<CPlantRedFire*>(e->obj))
		e->obj->Delete();

	if (e->obj->IsBlocking()) isCollidable = false;
}
void CHammer::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - HAMMER_BBOX_WIDTH / 2;
	t = y - HAMMER_BBOX_HEIGHT / 2;
	r = l + HAMMER_BBOX_WIDTH;
	b = t + HAMMER_BBOX_HEIGHT;
}
