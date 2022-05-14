#include "KoopaRedNormal.h"

#include "Mario.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "PlantRedFire.h"

#include "BrickQuestionMark.h"
#include "PlatformGhost.h"
#include "DeadZone.h"

#include "MushroomBig.h"
#include "Leaf.h"

CKoopaRedNormal::CKoopaRedNormal(float x, float y, const LPPLAYSCENE& currentScene)
	:CGameObject(x, y), currentScene(currentScene)
{
	isShell = false;
	isBeingHeld = false;
	shell_start = -1;
	nx = -1;
	this->ay = KOOPA_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopaRedNormal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING)
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


void CKoopaRedNormal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	if (state == KOOPA_STATE_WALKING &&
		attachedBBox != NULL && 
		attachedBBox->GetState() == ATTACHED_BBOX_STATE_FALL)
	{
		vx = -vx;
		nx = -nx;
		attachedBBox->Delete();
		attachedBBox = new CAttachedBBox(x + nx * KOOPA_NORMAL_BBOX_WIDTH, y, vx, vy);
		this->currentScene->AddObject(attachedBBox);
	}

	if (isShell && GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT &&
		state != KOOPA_STATE_SHELL_DOWNSIDE_MOVING &&
		state != KOOPA_STATE_SHELL_UPSIDE_MOVING)
	{
		if (isBeingHeld)
		{
			CMario* mario = (CMario*)this->currentScene->GetPlayer();
			mario->ReleaseHeldShell();
			isBeingHeld = false;
		}
		SetState(KOOPA_STATE_WALKING);
	}
			

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//if (isBeingHeld)
	//DebugOut(L"[INFO] Koopa x : %0.5f, koopa y : %0.5f, vx: %0.5f, vy :%0.5f \n", x, y, vx, vy);
}


void CKoopaRedNormal::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopaRedNormal::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CKoopaRedNormal*>(e->obj)) return;
	if (dynamic_cast<CMario*>(e->obj)) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

	if (dynamic_cast<CPlatformGhost*>(e->obj))
		OnCollisionWithPlatformGhost(e);

	else if (dynamic_cast<CGoomba*>(e->obj) || dynamic_cast<CGoombaRedWing*>(e->obj))
		OnCollisionWithGoomba(e);

	else if (dynamic_cast<CPlantRedFire*>(e->obj))
		OnCollisionWithPlantRedFire(e);

	else if (dynamic_cast<CBrickQuestionMark*>(e->obj))
		OnCollisionWithBrickQuestionMark(e);

	if (dynamic_cast<CDeadZone*>(e->obj))
		this->Delete();
}

void CKoopaRedNormal::OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{

		CPlatformGhost* platform = dynamic_cast<CPlatformGhost*>(e->obj);
		float platform_l, platform_t, platform_r, platform_b;
		platform->GetBoundingBox(platform_l, platform_t, platform_r, platform_b);

		float koopa_l, koopa_t, koopa_r, koopa_b;
		this->GetBoundingBox(koopa_l, koopa_t, koopa_r, koopa_b);

		// Have to directly change koopa's y because ghost platform is non-blocking, so the 
		// collisions with it are not handled by the framework (inside that, the coordinates
		// of the src_obj that collided with blocking things are automatically adjusted
		// to be the correct numbers using some calculations with BLOCK_PUSH_FACTOR)
		y = platform_t - (koopa_b - koopa_t)/ 2 - BLOCK_PUSH_FACTOR_GHOST_PLATFORM;

		vy = 0;
	}
}

void CKoopaRedNormal::OnCollisionWithPlantRedFire(LPCOLLISIONEVENT e)
{
	if (state != KOOPA_STATE_SHELL_DOWNSIDE_MOVING && state != KOOPA_STATE_SHELL_UPSIDE_MOVING)
		return;
	CPlantRedFire* plant = dynamic_cast<CPlantRedFire*>(e->obj);
	CGame::GetInstance()->UpdateScores(plant->GetScoresGivenWhenHit());
	plant->Delete();
}

void CKoopaRedNormal::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (state != KOOPA_STATE_SHELL_DOWNSIDE_MOVING && state != KOOPA_STATE_SHELL_UPSIDE_MOVING)
		return;
	if (dynamic_cast<CGoomba*>(e->obj))
	{
		CGoomba* goomba = (dynamic_cast<CGoomba*>(e->obj));
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());
		}
	}
	else if (dynamic_cast<CGoombaRedWing*>(e->obj))
	{
		CGoombaRedWing* goomba = (dynamic_cast<CGoombaRedWing*>(e->obj));
		if (goomba->GetState() != GOOMBA_RED_WING_STATE_DIE)
		{
			goomba->SetState(GOOMBA_RED_WING_STATE_DIE);
			CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());
		}
	}


}

void CKoopaRedNormal::OnCollisionWithBrickQuestionMark(LPCOLLISIONEVENT e)
{
	if (state != KOOPA_STATE_SHELL_DOWNSIDE_MOVING && state != KOOPA_STATE_SHELL_UPSIDE_MOVING)
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
	e->obj->SetState(BRICK_STATE_HIT_BY_MARIO);
}

int CKoopaRedNormal::GetAniId()
{
	if (state == KOOPA_STATE_WALKING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_WALKING_LEFT;
		else return ID_ANI_KOOPA_WALKING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_DOWNSIDE_STILL ||
		state == KOOPA_STATE_SHELL_DOWNSIDE_MARIO_HOLD)
		return ID_ANI_KOOPA_SHELL_DOWNSIDE_STILL;

	if (state == KOOPA_STATE_SHELL_UPSIDE_STILL ||
		state == KOOPA_STATE_SHELL_UPSIDE_MARIO_HOLD)
		return ID_ANI_KOOPA_SHELL_UPSIDE_STILL;

	if (state == KOOPA_STATE_SHELL_DOWNSIDE_MOVING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_LEFT;
		else return ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_UPSIDE_MOVING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_LEFT;
		else return ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_RIGHT;
	}

	return -1;
}

void CKoopaRedNormal::Render()
{
	int aniId = GetAniId();
	if (aniId == -1)
	{
		DebugOut(L"[ERROR] Render failed ! Can't find koopa_red_normal's ani \n");
		return;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();

	//DebugOutTitle(L"koopa's state: %d", state);
	//DebugOut(L"[INFO] Koopa's Render() has been called \n");
}

void CKoopaRedNormal::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case KOOPA_STATE_WALKING:
		{
			isShell = false;
			vx = nx * KOOPA_WALKING_SPEED;
			ay = KOOPA_GRAVITY;

			// To keep koopa on the platform after turning from shell to normal (just like with
			// mario_big to mario_small)
			y -= (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT);

			if (attachedBBox == NULL)
			{
				attachedBBox = new CAttachedBBox(x + nx * KOOPA_NORMAL_BBOX_WIDTH, y, vx, vy);
				this->currentScene->AddObject(attachedBBox);
			}
			break;
		}

		case KOOPA_STATE_SHELL_DOWNSIDE_STILL:
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

		case KOOPA_STATE_SHELL_DOWNSIDE_MARIO_HOLD:
		{
			isBeingHeld = true;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		}

		case KOOPA_STATE_SHELL_DOWNSIDE_MOVING:
		{
			isBeingHeld = false;
			y -= (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT);
			vx = nx * KOOPA_SHELL_MOVING_SPEED;
			ay = KOOPA_GRAVITY;
			break;
		}

		case KOOPA_STATE_SHELL_UPSIDE_STILL:
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

		case KOOPA_STATE_SHELL_UPSIDE_MARIO_HOLD:
		{
			isBeingHeld = true;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		}

		case KOOPA_STATE_SHELL_UPSIDE_MOVING:
		{
			isBeingHeld = false;
			y -= (KOOPA_NORMAL_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT);
			vx = nx * KOOPA_SHELL_MOVING_SPEED;
			ay = KOOPA_GRAVITY;
			break;
		}

	}
}
