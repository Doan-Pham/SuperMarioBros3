#include "Koopa.h"
#include "Mario.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "PlantRedFire.h"

#include "BrickQuestionMark.h"
#include "PBlock.h"
#include "BrickGlass.h"

#include "PlatformGhost.h"
#include "DeadZone.h"

#include "MushroomBig.h"
#include "Leaf.h"
#include "MushroomUp.h"
#include "Coin.h"
#include "SpecialEffectManager.h"
#include "PlantGreenNormal.h"


CKoopa::CKoopa(float x, float y, const LPPLAYSCENE& currentScene)
	:CGameObject(x, y), currentScene(currentScene)
{
	isShell = false;
	isBeingHeld = false;
	isOnPlatform = false;

	shell_start = -1;
	die_start = -1;
	nx = -1;
	this->ay = KOOPA_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_HOPPING)
	{
		left = x - KOOPA_NORMAL_BBOX_WIDTH / 2;
		top = y - KOOPA_NORMAL_BBOX_HEIGHT / 2;
		right = left + KOOPA_NORMAL_BBOX_WIDTH;
		bottom = top + KOOPA_NORMAL_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
}


void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	if (state == KOOPA_STATE_DIE && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (isOnPlatform && state == KOOPA_STATE_HOPPING) SetState(KOOPA_STATE_HOPPING);
	if (isOnPlatform && state == KOOPA_STATE_SHELL_STILL_UPSIDE)
	{
		vx = 0;
		vy = 0;
	}

	if (isShell && GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT &&
		state != KOOPA_STATE_SHELL_MOVING_DOWNSIDE &&
		state != KOOPA_STATE_SHELL_MOVING_UPSIDE)
	{
		if (isBeingHeld)
		{
			CMario* mario = (CMario*)this->currentScene->GetPlayer();
			mario->ReleaseHeldShell();
			isBeingHeld = false;
		}
		SetState(KOOPA_STATE_WALKING);
	}

	isOnPlatform = false;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	//if (isBeingHeld)
	//DebugOutTitle(L"Koopa x : %0.5f, koopa y : %0.5f, vx: %0.5f, vy :%0.5f , isBeingHeld : %i", x, y, vx, vy, isBeingHeld);
	//DebugOutTitle(L"Koopa x : %0.5f, koopa y : %0.5f, vx: %0.5f, vy :%0.5f , state : %i", x, y, vx, vy, state);
}


void CKoopa::OnNoCollision(DWORD dt)
{
	if (isBeingHeld) return;
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		isOnPlatform = true;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

	if (dynamic_cast<CDeadZone*>(e->obj))
		this->Delete();

	if (dynamic_cast<CPlatformGhost*>(e->obj) && !isBeingHeld)
		OnCollisionWithPlatformGhost(e);

	else if (dynamic_cast<CGoomba*>(e->obj) || dynamic_cast<CGoombaRedWing*>(e->obj))
		OnCollisionWithGoomba(e);

	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);

	else if (dynamic_cast<CPlantRedFire*>(e->obj) || dynamic_cast<CPlantGreenNormal*>(e->obj))
		OnCollisionWithPlant(e);

	else if (dynamic_cast<CBrickGlass*>(e->obj))
		OnCollisionWithBrickGlass(e);

	if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);

	else if (dynamic_cast<CBrickQuestionMark*>(e->obj))
		OnCollisionWithBrickQuestionMark(e);

}

void CKoopa::OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		isOnPlatform = true;

		CPlatformGhost* platform = dynamic_cast<CPlatformGhost*>(e->obj);
		float platform_l, platform_t, platform_r, platform_b;
		platform->GetBoundingBox(platform_l, platform_t, platform_r, platform_b);

		float koopa_l, koopa_t, koopa_r, koopa_b;
		this->GetBoundingBox(koopa_l, koopa_t, koopa_r, koopa_b);

		// Have to directly change koopa's y because ghost platform is non-blocking, so the 
		// collisions with it are not handled by the framework (inside that, the coordinates
		// of the src_obj that collided with blocking things are automatically adjusted
		// to be the correct numbers using some calculations with BLOCK_PUSH_FACTOR)
		y = platform_t - (koopa_b - koopa_t) / 2 - BLOCK_PUSH_FACTOR_GHOST_PLATFORM;

		vy = 0;
	}
}

void CKoopa::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	if (!(state == KOOPA_STATE_SHELL_MOVING_DOWNSIDE ||
		state == KOOPA_STATE_SHELL_MOVING_UPSIDE ||
		isBeingHeld))
		return;

	e->obj->SetState(PLANT_STATE_DIE);
	CGame::GetInstance()->UpdateScores(e->obj->GetScoresGivenWhenHit());
	CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_TAIL_ATTACK);

	if (isBeingHeld)
	{
		CMario* mario = (CMario*)this->currentScene->GetPlayer();
		mario->ReleaseHeldShell();
		SetState(KOOPA_STATE_DIE);
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (!(state == KOOPA_STATE_SHELL_MOVING_DOWNSIDE ||
		state == KOOPA_STATE_SHELL_MOVING_UPSIDE ||
		isBeingHeld))
		return;

	if (dynamic_cast<CGoomba*>(e->obj))
	{
		CGoomba* goomba = (dynamic_cast<CGoomba*>(e->obj));
		if (goomba->GetState() == GOOMBA_STATE_DIE ||
			goomba->GetState() == GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS)
			return;

		goomba->SetDirectionX(nx);
		goomba->SetState(GOOMBA_STATE_HIT_BY_DEADLY_ATTACKS);
		CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());

		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_TAIL_ATTACK);
	}
	else if (dynamic_cast<CGoombaRedWing*>(e->obj))
	{
		CGoombaRedWing* goomba = (dynamic_cast<CGoombaRedWing*>(e->obj));
		if (goomba->GetState() == GOOMBA_RED_WING_STATE_DIE ||
			goomba->GetState() == GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS)
			return;

		goomba->SetDirectionX(nx);
		goomba->SetState(GOOMBA_RED_WING_STATE_HIT_BY_DEADLY_ATTACKS);
		CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());

		CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_TAIL_ATTACK);
	}
	if (isBeingHeld)
	{
		CMario* mario = (CMario*)this->currentScene->GetPlayer();
		mario->ReleaseHeldShell();
		SetState(KOOPA_STATE_DIE);
	}
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	if (!(state == KOOPA_STATE_SHELL_MOVING_DOWNSIDE ||
		state == KOOPA_STATE_SHELL_MOVING_UPSIDE ||
		isBeingHeld))
		return;

	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	koopa->SetDirectionX(nx);
	koopa->SetState(KOOPA_STATE_DIE);

	CGame::GetInstance()->UpdateScores(koopa->GetScoresGivenWhenHit());
	CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_TAIL_ATTACK);
	if (isBeingHeld)
	{
		CMario* mario = (CMario*)this->currentScene->GetPlayer();
		mario->ReleaseHeldShell();
		SetState(KOOPA_STATE_DIE);
	}
}

void CKoopa::OnCollisionWithBrickQuestionMark(LPCOLLISIONEVENT e)
{
	if (state != KOOPA_STATE_SHELL_MOVING_DOWNSIDE && state != KOOPA_STATE_SHELL_MOVING_UPSIDE)
		return;
	CBrickQuestionMark* brick = dynamic_cast<CBrickQuestionMark*>(e->obj);
	if (brick->IsHidingItem())
	{
		float brick_x, brick_y;
		brick->GetPosition(brick_x, brick_y);

		CItem* hiddenItem;

		// TODO: Need a better way than referencing like this
		CMario* mario = (CMario*)this->currentScene->GetPlayer();
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			hiddenItem = new CMushroomBig(brick_x, brick_y);
		else
			hiddenItem = new CLeaf(brick_x, brick_y);

		brick->AddHiddenItem(hiddenItem);
		this->currentScene->AddObject(hiddenItem);
	}
	if (!brick->IsContentGiven())
	{
		CGame::GetInstance()->UpdateScores(brick->GetScoresGivenWhenHit());
		CGame::GetInstance()->UpdateCoins(brick->GetCoinsGivenWhenHit());
	}
	e->obj->SetState(BRICK_STATE_HIT_BY_MARIO);
}

void CKoopa::OnCollisionWithBrickGlass(LPCOLLISIONEVENT e)
{
	if (state != KOOPA_STATE_SHELL_MOVING_DOWNSIDE && state != KOOPA_STATE_SHELL_MOVING_UPSIDE)
		return;

	CBrickGlass* brick = dynamic_cast<CBrickGlass*>(e->obj);
	if (brick->GetState() == BRICK_STATE_BECOME_COIN) return;

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
	brick->SetState(BRICK_STATE_HIT_BY_MARIO);
}

void CKoopa::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	if (state != KOOPA_STATE_SHELL_MOVING_DOWNSIDE && state != KOOPA_STATE_SHELL_MOVING_UPSIDE)
		return;

	CCoin* coin = dynamic_cast<CCoin*>(e->obj);
	if (coin->GetState() == COIN_STATE_BECOME_BRICK)
	{
		e->obj->SetState(COIN_STATE_AS_BRICK_HIT_BY_MARIO);
	}
}

void CKoopa::SetState(int state)
{
	if (this->state == KOOPA_STATE_DIE) return;
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
	{
		isBeingHeld = false;
		isShell = false;
		vx = nx * KOOPA_WALKING_SPEED;
		ay = KOOPA_GRAVITY;

		// To keep koopa on the platform after turning from shell to normal (just like with
		// mario_big to mario_small)
		y -= (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT);

		if (attachedBBox == NULL && currentScene != NULL)
		{
			attachedBBox = new CAttachedBBox(x + nx * KOOPA_NORMAL_BBOX_WIDTH, y, vx, vy);
			this->currentScene->AddObject(attachedBBox);
		}
		break;
	}

	case KOOPA_STATE_SHELL_STILL_DOWNSIDE:
	{
		isShell = true;
		shell_start = GetTickCount64();
		vx = 0;

		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}
		break;
	}

	case KOOPA_STATE_SHELL_MARIO_HOLD_DOWNSIDE:
	{
		isBeingHeld = true;
		vx = 0;
		vy = 0;
		ay = 0;

		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}
		break;
	}

	case KOOPA_STATE_SHELL_MOVING_DOWNSIDE:
	{
		isBeingHeld = false;
		y -= (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT);
		vx = nx * KOOPA_SHELL_MOVING_SPEED;
		ay = KOOPA_GRAVITY;

		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}
		break;
	}

	case KOOPA_STATE_HIT_BY_MARIO_TAIL:
	{
		// Need to set this or else a mechanism in Update() method will stop shell from bouncing
		isOnPlatform = false;
		
		y -= (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT);
		vx = nx * KOOPA_SHELL_BOUNCE_SPPED_X;
		vy = -KOOPA_SHELL_BOUNCE_SPEED_Y;
		SetState(KOOPA_STATE_SHELL_STILL_UPSIDE);
	}

	case KOOPA_STATE_SHELL_STILL_UPSIDE:
	{
		isShell = true;
		shell_start = GetTickCount64();
		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}
		break;
	}

	case KOOPA_STATE_SHELL_MARIO_HOLD_UPSIDE:
	{
		isBeingHeld = true;
		vx = 0;
		vy = 0;
		ay = 0;

		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}
		break;
	}

	case KOOPA_STATE_SHELL_MOVING_UPSIDE:
	{
		isBeingHeld = false;
		y -= (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT);
		vx = nx * KOOPA_SHELL_MOVING_SPEED;
		ay = KOOPA_GRAVITY;

		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}
		break;
	}

	case KOOPA_STATE_HOPPING:
	{
		isBeingHeld = false;
		vy = -KOOPA_HOPPING_SPEED;
		ay = KOOPA_GRAVITY;

		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}
		break;
	}

	case KOOPA_STATE_DIE:
	{
		if (attachedBBox != NULL)
		{
			attachedBBox->Delete();
			attachedBBox = NULL;
		}

		die_start = GetTickCount64();
		vx = nx * KOOPA_SHELL_BOUNCE_SPPED_X;
		vy = -KOOPA_SHELL_BOUNCE_SPEED_Y/2;
		ay = KOOPA_GRAVITY;
		if (!isBeingHeld)
			CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_SCORES_APPEAR, KOOPA_SCORES_GIVEN_WHEN_HIT);
		isBeingHeld = false;
		break;
	}
	}
}
