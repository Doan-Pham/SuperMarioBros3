#include "AttackBBox.h"

#include "debug.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "PlantRedFire.h"
#include "KoopaRedNormal.h"

CAttackBBox::CAttackBBox(float x, float y, float vx, float vy, int width, int height)
	:CGameObject(x, y)
{
	this->vx = vx;
	this->vy = vy;
	this->width = width;
	this->height = height;
}


void CAttackBBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - width / 2;
	top = y - height / 2;
	right = left + width;
	bottom = top + height;
}


void CAttackBBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (CCollision::GetInstance()->RegularAABB(this, coObjects->at(i)))
			OnCollisionWith(new CCollisionEvent(-1, -1, -1, -1, -1, this, coObjects->at(i)));
	}
	//DebugOutTitle(L"attached bbox: x %0.5f , y %0.5f, vx %0.5f, vy %0.5f", x, y, vx, vy);
}


void CAttackBBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj))
		e->obj->SetState(GOOMBA_STATE_DIE);

	if (dynamic_cast<CGoombaRedWing*>(e->obj))
		e->obj->SetState(GOOMBA_RED_WING_STATE_DIE);

	if (dynamic_cast<CKoopaRedNormal*>(e->obj))
		e->obj->SetState(KOOPA_STATE_DIE);

	if (dynamic_cast<CPlantRedFire*>(e->obj))
		e->obj->Delete();

}
