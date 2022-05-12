#include "KoopaRedNormal.h"


CKoopaRedNormal::CKoopaRedNormal(float x, float y) :CGameObject(x, y)
{
	this->ay = KOOPA_NORMAL_GRAVITY;
	//SetState(KOOPA_NORMAL_STATE_WALKING);
}

void CKoopaRedNormal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//if (state == KOOPA_NORMAL_STATE_DIE)
	//{
	//	left = x - KOOPA_NORMAL_BBOX_WIDTH / 2;
	//	top = y - KOOPA_NORMAL_BBOX_HEIGHT_DIE / 2;
	//	right = left + KOOPA_NORMAL_BBOX_WIDTH;
	//	bottom = top + KOOPA_NORMAL_BBOX_HEIGHT_DIE;
	//}
	//else
	//{
	//	left = x - KOOPA_NORMAL_BBOX_WIDTH / 2;
	//	top = y - KOOPA_NORMAL_BBOX_HEIGHT / 2;
	//	right = left + KOOPA_NORMAL_BBOX_WIDTH;
	//	bottom = top + KOOPA_NORMAL_BBOX_HEIGHT;
	//}
}

void CKoopaRedNormal::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopaRedNormal::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopaRedNormal*>(e->obj)) return;
	//if (dynamic_cast<CMario*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopaRedNormal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopaRedNormal::Render()
{
	//int aniId = ID_ANI_KOOPA_NORMAL_WALKING;
	//if (state == KOOPA_NORMAL_STATE_DIE)
	//{
	//	aniId = ID_ANI_KOOPA_NORMAL_DIE;
	//}

	//CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopaRedNormal::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		//case KOOPA_NORMAL_STATE_DIE:
		//	die_start = GetTickCount64();
		//	y += (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_NORMAL_BBOX_HEIGHT_DIE) / 2;
		//	vx = 0;
		//	vy = 0;
		//	ay = 0;
		//	break;
		//case KOOPA_NORMAL_STATE_WALKING:
		//	vx = -KOOPA_NORMAL_WALKING_SPEED;
		//	break;
		//}
	}
}
