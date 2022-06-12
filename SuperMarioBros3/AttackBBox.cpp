#include "AttackBBox.h"

#include "debug.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "PlantRedFire.h"
#include "PlantGreenNormal.h"
#include "Koopa.h"
#include "PlayScene.h"
#include "BrickQuestionMark.h"
#include "Leaf.h"
#include "MushroomUp.h"
#include "BrickGlass.h"
#include "PBlock.h"
#include "Coin.h"
#include "SpecialEffectManager.h"

CAttackBBox::CAttackBBox(float x, float y, float vx, float vy, float width, float height, 
	const LPPLAYSCENE currentScene, int nx) : CGameObject(x, y), currentScene(currentScene)
{
	this->vx = vx;
	this->vy = vy;
	this->width = width;
	this->height = height;
	this->nx = nx;
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
			OnCollisionWith(new CCollisionEvent(-1, -1, -1, -1, -1, coObjects->at(i), this));
		//DebugOut(L"Regular AABB happened \n");
	}
	//DebugOutTitle(L"attached bbox: x %0.5f , y %0.5f, vx %0.5f, vy %0.5f", x, y, vx, vy);
}


void CAttackBBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) return;
	if (dynamic_cast<CAttackBBox*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj))
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		if (goomba->GetState() == GOOMBA_STATE_DIE ||
			goomba->GetState() == GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS)
			return;

		goomba->SetDirectionX(nx);
		goomba->SetState(GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS);
		CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());

		CSpecialEffectManager::CreateSpecialEffect(x + nx * width / 2, y, EFFECT_TYPE_TAIL_ATTACK);
	}


	else if (dynamic_cast<CGoombaRedWing*>(e->obj))
	{
		CGoombaRedWing* goomba = dynamic_cast<CGoombaRedWing*>(e->obj);
		if (goomba->GetState() == GOOMBA_RED_WING_STATE_DIE ||
			goomba->GetState() == GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS) 
			return;

		goomba->SetDirectionX(nx);
		goomba->SetState(GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS);
		CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());

		CSpecialEffectManager::CreateSpecialEffect(x + nx * width / 2, y, EFFECT_TYPE_TAIL_ATTACK);
	}
		

	else if (dynamic_cast<CKoopa*>(e->obj))
	{
		CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
		koopa->SetDirection(nx);
		koopa->SetState(KOOPA_STATE_SHELL_STILL_UPSIDE);
		CSpecialEffectManager::CreateSpecialEffect(x + nx * width / 2, y, EFFECT_TYPE_TAIL_ATTACK);
	}
		

	else if (dynamic_cast<CPlantRedFire*>(e->obj) || dynamic_cast<CPlantGreenNormal*>(e->obj))
	{
		e->obj->SetState(PLANT_STATE_DIE);
		CSpecialEffectManager::CreateSpecialEffect(x + nx * width / 2, y, EFFECT_TYPE_TAIL_ATTACK);
	}


	else if (dynamic_cast<CBrickQuestionMark*>(e->obj))
	{
		CBrickQuestionMark* brick = dynamic_cast<CBrickQuestionMark*>(e->obj);
		if (brick->IsHidingItem())
		{
			float brick_x, brick_y;
			brick->GetPosition(brick_x, brick_y);

			CItem* hiddenItem = new CLeaf(brick_x, brick_y);

			brick->AddHiddenItem(hiddenItem);
			currentScene->AddObject(hiddenItem);
		}
		if (!brick->IsContentGiven())
		{
			CGame::GetInstance()->UpdateScores(brick->GetScoresGivenWhenHit());
			CGame::GetInstance()->UpdateCoins(brick->GetCoinsGivenWhenHit());
		}
		brick->SetState(BRICK_STATE_HIT_BY_MARIO);

	}

	else if (dynamic_cast<CBrickGlass*>(e->obj))
	{
		CBrickGlass* brick = dynamic_cast<CBrickGlass*>(e->obj);

		float brick_x, brick_y;
		brick->GetPosition(brick_x, brick_y);
		if (brick->IsHidingUpMushroom())
		{
			CItem* hiddenItem = new CMushroomUp(brick_x, brick_y);
			brick->AddHiddenItem(hiddenItem);
			this->currentScene->AddObject(hiddenItem);
		}
		else if (brick->IsHidingPBlock() && !brick->IsHitByMario())
		{
			CPBlock* newPBlock = new CPBlock(brick_x, brick_y - BLOCK_BBOX_HEIGHT);
			this->currentScene->AddObject(newPBlock);
		}
		e->obj->SetState(BRICK_STATE_HIT_BY_MARIO);
	}

	else if (dynamic_cast<CCoin*>(e->obj) && e->obj->GetState() == COIN_STATE_BECOME_BRICK) 
	{
		CGame::GetInstance()->UpdateScores(e->obj->GetScoresGivenWhenHit());
		CGame::GetInstance()->UpdateCoins(e->obj->GetCoinsGivenWhenHit());
		e->obj->Delete();
	}
}


