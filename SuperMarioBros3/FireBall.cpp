#include "FireBall.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "Koopa.h"
#include "PlantRedFire.h"
#include "PlantGreenNormal.h"
#include "SpecialEffectManager.h"
#include "PlatformGhost.h"

CFireBall::CFireBall(float x, float y, int nx): CGameObject(x, y)
{
	this->nx = nx;
	vx = nx * FIREBALL_SPEED_X;
	vy = 0;
	ay = FIREBALL_GRAVITY;
	isDestroyed = false;
}

void CFireBall::Render()
{
	if (isDestroyed) return;
	CAnimations::GetInstance()->Get(ID_ANI_FIREBALL_MOVING)->Render(x, y);
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed) return;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CFireBall::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireBall::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && (e->obj->IsBlocking() || dynamic_cast<CPlatformGhost*>(e->obj)))
	{
		vy = -FIREBALL_SPEED_Y;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
		isDestroyed = true;
		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_FIREBALL_COLLIDE);
	}

	if (dynamic_cast<CGoomba*>(e->obj))
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		if (goomba->GetState() == GOOMBA_STATE_DIE ||
			goomba->GetState() == GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS)
			return;

		goomba->SetDirectionX(nx);
		goomba->SetState(GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS);
		CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());

		isDestroyed = true;
		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_FIREBALL_COLLIDE);
	}

	if (dynamic_cast<CGoombaRedWing*>(e->obj))
	{
		CGoombaRedWing* goomba = dynamic_cast<CGoombaRedWing*>(e->obj);
		if (goomba->GetState() == GOOMBA_RED_WING_STATE_DIE ||
			goomba->GetState() == GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS)
			return;

		goomba->SetDirectionX(nx);
		goomba->SetState(GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS);
		CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());

		isDestroyed = true;
		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_FIREBALL_COLLIDE);
	}

	if (dynamic_cast<CKoopa*>(e->obj))
	{
		CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
		koopa->SetDirectionX(nx);
		koopa->SetState(KOOPA_STATE_DIE);

		CGame::GetInstance()->UpdateScores(koopa->GetScoresGivenWhenHit());

		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_FIREBALL_COLLIDE);
		isDestroyed = true;
	}

	if (dynamic_cast<CPlantRedFire*>(e->obj) || dynamic_cast<CPlantGreenNormal*>(e->obj))
	{
		e->obj->SetState(PLANT_STATE_DIE);
		isDestroyed = true;

		CGame::GetInstance()->UpdateScores(e->obj->GetScoresGivenWhenHit());
		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_FIREBALL_COLLIDE);
	}

}
void CFireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIREBALL_BBOX_WIDTH / 2;
	t = y - FIREBALL_BBOX_HEIGHT / 2;
	r = l + FIREBALL_BBOX_WIDTH;
	b = t + FIREBALL_BBOX_HEIGHT;
}
