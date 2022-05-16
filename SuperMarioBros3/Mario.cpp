#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Collision.h"

#include "Item.h"
#include "MushroomBig.h"
#include "Leaf.h"

#include "BrickQuestionMark.h"
#include "PlatformGhost.h"
#include "Portal.h"
#include "DeadZone.h"

#include "FireShot.h"
#include "FireBall.h"

#include "Goomba.h"
#include "PlantRedFire.h"
#include "KoopaRedNormal.h"
#include "GoombaRedWing.h"

CMario::CMario(float x, float y, const LPPLAYSCENE& currentScene)
	: CGameObject(x, y), currentScene(currentScene)
{
	isSitting = false;
	isFlying = false;
	isTrulyFalling = false;
	isTailWhipping = false;
	isKicking = false;
	isReadyToHoldShell = false;
	isHoldingShell = false;
	isThrowingFireball = false;
	isThrowingHammer = false;

	maxVx = 0.0f;
	ax = 0.0f;
	ay = MARIO_GRAVITY;

	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	untouchable_start = -1;

	fly_total_start = -1;
	fly_individual_start = -1;

	tail_wag_start = -1;
	tail_whip_start = -1;;

	throw_fireball_start = -1;
	throw_hammer_start = -1;
	isOnPlatform = false;

	pMeter = new CPMeter();
	shellBeingHeld = NULL;
	raccoon_tail = NULL;
}


#pragma region UPDATE_COLLISION_HANDLING

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	ULONGLONG now = GetTickCount64();

	// reset untouchable timer if untouchable time has passed
	if (now - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// If mario's flying and he doesn't hit the "S" key fast enough, make him fall, but he can
	// still fly again
	if (isFlying && ((now - fly_individual_start) > MARIO_WAIT_BEFORE_FALLING_AFTER_FLY))
	{
		SetState(MARIO_STATE_FALLING);
		//DebugOut(L"Mario's state set to falling after flying \n");
	}

	// If mario's falling and he doesn't hit the "S" key fast enough, make him fall, but he can still
	// wag tail again to fall slowly
	if ((isTrulyFalling && ((now - tail_wag_start) > MARIO_WAIT_BEFORE_FALLING_AFTER_TAIL_WAG))
		|| isOnPlatform)
	{
		SetState(MARIO_STATE_FALLING);
		//DebugOut(L"Mario's state set to falling after wagging tail \n");
	}

	// The total fly time for mario is over, make him fall and not allow him to fly again
	if (isFlying && now - fly_total_start > MARIO_MAX_TOTAL_FLY_TIME)
	{
		isFlying = false;
		isTrulyFalling = true;
	}

	// Reset flags and fly timer
	if (isOnPlatform)
	{
		fly_total_start = -1;
		ay = MARIO_GRAVITY;

		// If we don't set this, mario can't start flying because he will fail the isTrulyFalling()
		// check in KeyEventHandler
		isTrulyFalling = false;
	}

	// If we don't check for isFlying, mario won't be able to hit "S" to fly again after falling
	// down a bit
	else if (vy > 0 && !isFlying) isTrulyFalling = true;

	// Setting this will let mario whip tail for a certain amount of time before stopping
	if (isTailWhipping && ((now - tail_whip_start) > MARIO_RACCOON_TAIL_WHIP_ANI_TIMEOUT))
	{
		isTailWhipping = false;

		if (raccoon_tail != NULL)
		{
			raccoon_tail->Delete();
			raccoon_tail = NULL;
		}
	}

	// Setting this allows the fireball_throwing animation to last a bit longer
	if (isThrowingFireball && ((now - throw_fireball_start) > MARIO_FIRE_THROW_FIREBALL_ANI_TIMEOUT))
	{
		isThrowingFireball = false;
	}
	for (unsigned int i = 0; i < fireBalls.size(); i++)
	{
		// Let mario handle the case where fireballs go off screen
		if (!fireBalls[i]->IsDestroyed())
		{
			float fireball_x, fireball_y, cam_x, cam_y;
			fireBalls[i]->GetPosition(fireball_x, fireball_y);
			CGame::GetInstance()->GetCamPos(cam_x, cam_y);

			if ((fireball_x < cam_x) || (fireball_x > (cam_x + SCREEN_WIDTH)) ||
				(fireball_y < cam_y) || (fireball_y > (cam_y + SCREEN_HEIGHT)))
			{
				fireBalls[i]->Delete();
				fireBalls.erase(fireBalls.begin() + i);
			}
		}
		else
		{
			fireBalls[i]->Delete();
			fireBalls.erase(fireBalls.begin() + i);
		}

	}

	// Setting this allows the hammer_throwing animation to last a bit longer
	if (isThrowingHammer && ((now - throw_hammer_start) > MARIO_FIRE_THROW_FIREBALL_ANI_TIMEOUT))
	{
		isThrowingHammer = false;
	}
	for (unsigned int i = 0; i < hammers.size(); i++)
	{

		float hammer_x, hammer_y, cam_x, cam_y;
		hammers[i]->GetPosition(hammer_x, hammer_y);
		CGame::GetInstance()->GetCamPos(cam_x, cam_y);

		if ((hammer_x < cam_x) || (hammer_x > (cam_x + SCREEN_WIDTH)) ||
			(hammer_y > (cam_y + SCREEN_HEIGHT)))
		{
			hammers[i]->Delete();
			hammers.erase(hammers.begin() + i);
		}
	}

	isKicking = false;
	isOnPlatform = false;


	CCollision::GetInstance()->Process(this, dt, coObjects);


	//DebugOutTitle(L"isTailWhipping %d, nx %d ", isTailWhipping, nx);
	//DebugOutTitle(L"Current state %d", this->state);

	if (isHoldingShell)
		shellBeingHeld->SetPosition(x + nx * GetBBoxWidth() / 2, y);

	if (isTailWhipping)
	{
		raccoon_tail->SetSpeed(vx, vy);
		raccoon_tail->SetPosition(x + nx * GetBBoxWidth() / 2, y);
	}
		
	//DebugOut(L"level: %d, mario_x : %0.5f, mario_y: %0.5f, mario_vx: %0.5f, ax : %0.5f \n", level, x, y, vx, ax);
	//DebugOutTitle(L"state: %d,  mario_vy: %0.5f, ay : %0.5f ", state, vy, ay);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		if (e->ny < 0) isOnPlatform = true;
		vy = 0;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CPlatformGhost*>(e->obj))
		OnCollisionWithPlatformGhost(e);

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);

	if (dynamic_cast<CGoombaRedWing*>(e->obj))
		OnCollisionWithGoombaRedWing(e);

	if (dynamic_cast<CKoopaRedNormal*>(e->obj))
		OnCollisionWithKoopaNormal(e);

	else if (dynamic_cast<CItem*>(e->obj))
		OnCollisionWithItem(e);

	else if (dynamic_cast<CFireShot*>(e->obj))
		OnCollisionWithFireShot(e);

	else if (dynamic_cast<CPlantRedFire*>(e->obj))
		OnCollisionWithPlant(e);

	else if (dynamic_cast<CBrickQuestionMark*>(e->obj))
		OnCollisionWithBrickQuestionMark(e);

	else if (dynamic_cast<CDeadZone*>(e->obj))
		OnCollisionWithDeadZone(e);

	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level < MARIO_LEVEL_BIG)
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					if (isTailWhipping && e->nx != 0 && e->ny == 0)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());
					}
					else
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
				}
			}
		}
	}
}

void CMario::OnCollisionWithGoombaRedWing(LPCOLLISIONEVENT e)
{
	CGoombaRedWing* goomba = dynamic_cast<CGoombaRedWing*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_RED_WING_STATE_DIE)
		{
			if (goomba->GetState() == GOOMBA_RED_WING_STATE_WALKING_CLOSED_WING ||
				goomba->GetState() == GOOMBA_RED_WING_STATE_HOPPING ||
				goomba->GetState() == GOOMBA_RED_WING_STATE_JUMPING)
			{
				goomba->SetState(GOOMBA_RED_WING_STATE_WALKING_NORMAL);
			}
			else goomba->SetState(GOOMBA_RED_WING_STATE_DIE);

			CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_RED_WING_STATE_DIE)
			{
				if (level < MARIO_LEVEL_BIG)
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					if (isTailWhipping && e->nx != 0 && e->ny == 0)
					{
						goomba->SetState(GOOMBA_RED_WING_STATE_DIE);
						CGame::GetInstance()->UpdateScores(goomba->GetScoresGivenWhenHit());
					}
					else
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopaNormal(LPCOLLISIONEVENT e)
{
	CKoopaRedNormal* koopa = dynamic_cast<CKoopaRedNormal*>(e->obj);

	// jump on top >> kill koopa and deflect a bit 
	if (e->ny < 0)
	{
		switch (koopa->GetState())
		{
		case KOOPA_STATE_WALKING:
		case KOOPA_STATE_SHELL_DOWNSIDE_MOVING:
			koopa->SetState(KOOPA_STATE_SHELL_DOWNSIDE_STILL);
			break;

		case KOOPA_STATE_SHELL_UPSIDE_MOVING:
			koopa->SetState(KOOPA_STATE_SHELL_UPSIDE_STILL);
			break;

		case KOOPA_STATE_SHELL_DOWNSIDE_STILL:
			koopa->SetState(KOOPA_STATE_SHELL_DOWNSIDE_MOVING);
			break;

		case KOOPA_STATE_SHELL_UPSIDE_STILL:
			koopa->SetState(KOOPA_STATE_SHELL_UPSIDE_MOVING);
			break;
		}
		CGame::GetInstance()->UpdateScores(koopa->GetScoresGivenWhenHit());
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else // hit by koopa
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() == KOOPA_STATE_WALKING ||
				koopa->GetState() == KOOPA_STATE_SHELL_DOWNSIDE_MOVING ||
				koopa->GetState() == KOOPA_STATE_SHELL_UPSIDE_MOVING)
			{
				if (level < MARIO_LEVEL_BIG)
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					if (isTailWhipping && e->nx != 0 && e->ny == 0)
					{
						koopa->SetState(KOOPA_STATE_SHELL_UPSIDE_STILL);
					}
					else
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
				}
			}
			else
			{
				// Mario collides with shell when player's pressing "A" key, mario holds shell
				if (isReadyToHoldShell)
				{
					isHoldingShell = true;
					shellBeingHeld = koopa;
					if (koopa->GetState() == KOOPA_STATE_SHELL_DOWNSIDE_STILL)
						koopa->SetState(KOOPA_STATE_SHELL_DOWNSIDE_MARIO_HOLD);

					else if (koopa->GetState() == KOOPA_STATE_SHELL_UPSIDE_STILL)
						koopa->SetState(KOOPA_STATE_SHELL_UPSIDE_MARIO_HOLD);
				}

				// Mario collides with shell when player's not pressing "A", mario kicks shell
				else
				{
					isHoldingShell = false;
					isKicking = true;
					koopa->SetDirection(this->nx);
					if (koopa->GetState() == KOOPA_STATE_SHELL_DOWNSIDE_STILL)
						koopa->SetState(KOOPA_STATE_SHELL_DOWNSIDE_MOVING);
					else if (koopa->GetState() == KOOPA_STATE_SHELL_UPSIDE_STILL)
						koopa->SetState(KOOPA_STATE_SHELL_UPSIDE_MOVING);
				}
			}
		}
	}
}

void CMario::KickHeldShell()
{
	isHoldingShell = false;
	isKicking = true;
	shellBeingHeld->SetDirection(this->nx);
	if (shellBeingHeld->GetState() == KOOPA_STATE_SHELL_DOWNSIDE_MARIO_HOLD)
		shellBeingHeld->SetState(KOOPA_STATE_SHELL_DOWNSIDE_MOVING);
	else if (shellBeingHeld->GetState() == KOOPA_STATE_SHELL_UPSIDE_MARIO_HOLD)
		shellBeingHeld->SetState(KOOPA_STATE_SHELL_UPSIDE_MOVING);
	shellBeingHeld = NULL;
}

void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	CPlantRedFire* plant = dynamic_cast<CPlantRedFire*>(e->obj);
	if (untouchable == 0)
	{
		if (level < MARIO_LEVEL_BIG)
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			if (isTailWhipping && e->nx != 0 && e->ny == 0)
			{
				CGame::GetInstance()->UpdateScores(plant->GetScoresGivenWhenHit());
				plant->Delete();
			}
			else
			{
				level = MARIO_LEVEL_BIG;
				StartUntouchable();
			}
		}

	}
}

void CMario::OnCollisionWithItem(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CLeaf*>(e->obj)) SetLevel(MARIO_LEVEL_RACCOON);
	else if (dynamic_cast<CMushroomBig*>(e->obj)) SetLevel(MARIO_LEVEL_BIG);

	CGame::GetInstance()->UpdateScores(e->obj->GetScoresGivenWhenHit());
	CGame::GetInstance()->UpdateCoins(e->obj->GetCoinsGivenWhenHit());
	e->obj->Delete();
}

void CMario::OnCollisionWithFireShot(LPCOLLISIONEVENT e)
{
	if (untouchable == 0)
	{
		if (level < MARIO_LEVEL_BIG)
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_HAMMER)
		{
			if (isSitting) e->obj->Delete();
			else
			{
				level = MARIO_LEVEL_BIG;
				StartUntouchable();
			}
		}
		else
		{
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
	}
}

void CMario::OnCollisionWithDeadZone(LPCOLLISIONEVENT e)
{
	DebugOut(L">>> Mario DIE >>> \n");
	SetState(MARIO_STATE_DIE);
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithBrickQuestionMark(LPCOLLISIONEVENT e)
{

	if ((e->ny > 0 && e->nx == 0) || (e->nx != 0 && e->ny == 0 && isTailWhipping))
	{
		CBrickQuestionMark* brick = dynamic_cast<CBrickQuestionMark*>(e->obj);
		if (brick->IsHidingItem())
		{
			float brick_x, brick_y;
			brick->GetPosition(brick_x, brick_y);

			CItem* hiddenItem;

			if (level == MARIO_LEVEL_SMALL)
				hiddenItem = new CMushroomBig(brick_x, brick_y);
			else
				hiddenItem = new CLeaf(brick_x, brick_y);

			brick->AddHiddenItem(hiddenItem);
			this->currentScene->AddObject(hiddenItem);
		}
		e->obj->SetState(BRICK_STATE_HIT_BY_MARIO);
	}
}

void CMario::OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		isOnPlatform = true;

		CPlatformGhost* platform = dynamic_cast<CPlatformGhost*>(e->obj);
		float platform_l, platform_t, platform_r, platform_b;

		platform->GetBoundingBox(platform_l, platform_t, platform_r, platform_b);

		// Have to directly change mario's y because ghost platform is non-blocking, so the 
		// collisions with it are not handled by the framework (inside that, the coordinates
		// of the src_obj that collided with blocking things are automatically adjusted
		// to be the correct numbers using some calculations with BLOCK_PUSH_FACTOR)
		y = platform_t - GetBBoxHeight() / 2 - BLOCK_PUSH_FACTOR_GHOST_PLATFORM;

		vy = 0;
	}
}

#pragma endregion


#pragma region RENDER


//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				};
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else
				{
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				}
			}

	if (isKicking)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
		else aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
	}

	if (isHoldingShell)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_SMALL_HOLD_RIGHT;
		else aniId = ID_ANI_MARIO_SMALL_HOLD_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else
				{
					// If p-meter is not changing or if it is increasing but not fully charged, mario's
					// animation will be that of walking
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else
				{
					aniId = ID_ANI_MARIO_WALKING_LEFT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_RUNNING_LEFT;
				}
			}
	if (isKicking)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_KICK_RIGHT;
		else aniId = ID_ANI_MARIO_KICK_LEFT;
	}

	if (isHoldingShell)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_HOLD_RIGHT;
		else aniId = ID_ANI_MARIO_HOLD_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

//
// Get animation ID for raccoon Mario
//
int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isFlying)//abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		// If mario is not flying but his vy < 0, this means he's jumping
		else if (vy < 0)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
		// If mario is not flying, but he's in the sky, he's falling down
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_TAIL_WAG_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_TAIL_WAG_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				};
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
				else
				{
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				}
			}
	if (isTailWhipping)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_TAIL_WHIP_RIGHT;
		else aniId = ID_ANI_MARIO_RACCOON_TAIL_WHIP_LEFT;
	}

	// Kicking has higher render priority than tail whipping
	if (isKicking)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_KICK_RIGHT;
		else aniId = ID_ANI_MARIO_RACCOON_KICK_LEFT;
	}

	if (isHoldingShell)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_HOLD_RIGHT;
		else aniId = ID_ANI_MARIO_RACCOON_HOLD_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

	return aniId;
}

//
// Get animdation ID for fire Mario
//
int CMario::GetAniIdFire()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_FIRE_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_FIRE_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_FIRE_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_FIRE_BRACE_RIGHT;
				else
				{
					// If p-meter is not changing or if it is increasing but not fully charged, mario's
					// animation will be that of walking
					aniId = ID_ANI_MARIO_FIRE_WALKING_RIGHT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_FIRE_RUNNING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_FIRE_BRACE_LEFT;
				else
				{
					aniId = ID_ANI_MARIO_FIRE_WALKING_LEFT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_FIRE_RUNNING_LEFT;
				}
			}
	if (isKicking)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_FIRE_KICK_RIGHT;
		else aniId = ID_ANI_MARIO_FIRE_KICK_LEFT;
	}

	if (isHoldingShell)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_FIRE_HOLD_RIGHT;
		else aniId = ID_ANI_MARIO_FIRE_HOLD_LEFT;
	}
	if (isThrowingFireball)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_FIRE_THROW_FIREBALL_RIGHT;
		else aniId = ID_ANI_MARIO_FIRE_THROW_FIREBALL_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_FIRE_IDLE_RIGHT;

	return aniId;
}

//
// Get animdation ID for Hammer Mario
//
int CMario::GetAniIdHammer()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_HAMMER_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_HAMMER_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_HAMMER_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_HAMMER_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_HAMMER_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_HAMMER_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_HAMMER_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_HAMMER_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_HAMMER_BRACE_RIGHT;
				else
				{
					// If p-meter is not changing or if it is increasing but not fully charged, mario's
					// animation will be that of walking
					aniId = ID_ANI_MARIO_HAMMER_WALKING_RIGHT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_HAMMER_RUNNING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_HAMMER_BRACE_LEFT;
				else
				{
					aniId = ID_ANI_MARIO_HAMMER_WALKING_LEFT;
					if (pMeter->isFullyCharged())
						aniId = ID_ANI_MARIO_HAMMER_RUNNING_LEFT;
				}
			}
	if (isKicking)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_HAMMER_KICK_RIGHT;
		else aniId = ID_ANI_MARIO_HAMMER_KICK_LEFT;
	}

	if (isHoldingShell)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_HAMMER_HOLD_RIGHT;
		else aniId = ID_ANI_MARIO_HAMMER_HOLD_LEFT;
	}
	if (isThrowingHammer)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_HAMMER_THROW_HAMMER_RIGHT;
		else aniId = ID_ANI_MARIO_HAMMER_THROW_HAMMER_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_HAMMER_IDLE_RIGHT;

	return aniId;
}
void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon();
	else if (level == MARIO_LEVEL_FIRE)
		aniId = GetAniIdFire();
	else if (level == MARIO_LEVEL_HAMMER)
		aniId = GetAniIdHammer();

	//DebugOutTitle(L"aniId: %d", aniId);
	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	//DebugOutTitle(L"mario_x : %0.5f, mario_y : %0.5f, vx: %0.5f, vy: %0.5f", x, y, vx, vy);
	//DebugOutTitle(L"mario_x : %0.5f, mario_y : %0.5f, cam_x: %0.5f, cam_y: %0.5f", x,y, cam_x, cam_y);
	//DebugOutTitle(L"Coins: %d", coin);
}

#pragma endregion


void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{

	case MARIO_STATE_IDLE:
	{
		if (isFlying) return;

		//if (level == MARIO_LEVEL_RACCOON)
		//	DebugOutTitle(L"STATE_IDLE HAS BEEN CALLED WHILE BEING RACCOON");
		ax = 0.0f;
		vx = 0.0f;
		isReadyToHoldShell = false;

		pMeter->SetState(P_METER_STATE_DECREASING);

		break;
	}

	case MARIO_STATE_SIT:
	{
		if (isOnPlatform && level != MARIO_LEVEL_SMALL && !isHoldingShell)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
			isReadyToHoldShell = false;
		}
		break;
	}

	case MARIO_STATE_SIT_RELEASE:
	{
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	}

	case MARIO_STATE_WALKING_RIGHT:
	{
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		isReadyToHoldShell = false;
		// As long as mario's still in the state flying, he can fly again, even if he's on the
		// platform
		if (isOnPlatform && !isFlying) pMeter->SetState(P_METER_STATE_DECREASING);
		break;
	}


	case MARIO_STATE_WALKING_LEFT:
	{
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;

		isReadyToHoldShell = false;
		// As long as mario's still in the state flying, he can fly again, even if he's on the
		// platform
		if (isOnPlatform && !isFlying) pMeter->SetState(P_METER_STATE_DECREASING);
		break;
	}

	case MARIO_STATE_RUNNING_RIGHT:
	{
		if (isSitting) break;

		// TODO: Maybe divide flying state into fly left and fly right
		// This is to keep mario's vx when flying to be at the same value
		if (isFlying)
		{
			SetState(MARIO_STATE_WALKING_RIGHT);
			break;
		}
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;

		isReadyToHoldShell = true;
		pMeter->SetState(P_METER_STATE_INCREASING);
		break;
	}


	case MARIO_STATE_RUNNING_LEFT:
	{
		if (isSitting) break;

		// This is to keep mario's vx when flying to be at the same value
		if (isFlying)
		{
			SetState(MARIO_STATE_WALKING_LEFT);
			break;
		}
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;

		isReadyToHoldShell = true;
		pMeter->SetState(P_METER_STATE_INCREASING);
		break;
	}

	case MARIO_STATE_JUMP:
	{
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;
	}

	case MARIO_STATE_RELEASE_JUMP:
	{
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;
	}

	case MARIO_STATE_FLY:
	{

		// fly_total_start being equal -1 means mario's on the platform and can start flying
		if (fly_total_start == -1)
		{
			fly_total_start = GetTickCount64();
			fly_individual_start = GetTickCount64();
			isFlying = true;

			// If we don't set this, the "isOnPlatform" check in Update() will resset fly_total_start
			// and prevent mario from flying
			isOnPlatform = false;

			// A boost vy to help mario overcome gravity when he takes off
			vy = -MARIO_TAKE_OFF_SPEED_Y;

			ay = 0;
			break;
		}

		if (!isFlying) return;
		fly_individual_start = GetTickCount64();
		vy = -MARIO_FLYING_SPEED_Y;

		// Set ay = 0, so that mario's not dragged down by gravity and feel like he's really flying
		// instead of just jumping infinitely
		ay = 0;
		break;
	}

	case MARIO_STATE_FALLING:
	{
		ay = MARIO_GRAVITY;
		break;
	}

	case MARIO_STATE_TAIL_WAGGING:
	{
		tail_wag_start = GetTickCount64();
		ay = MARIO_GRAVITY_SLOW_FALL;

		// Assign vy = 0 to avoid the stacking of vy from STATE_FALLING, this stacking will just
		// overwhelme the ay assigned here
		vy = 0;
		break;
	}

	case MARIO_STATE_TAIL_WHIPPING:
	{
		if (!isTailWhipping)
		{
			tail_whip_start = GetTickCount64();
			isTailWhipping = true;
			if (raccoon_tail == NULL)
			{
				raccoon_tail = new CAttackBBox(
					x + nx * GetBBoxWidth(), y, 
					vx, vy, 
					GetBBoxWidth(), GetBBoxHeight(),
					currentScene);

				this->currentScene->AddObject(raccoon_tail);
			}
		}
		break;
	}

	case MARIO_STATE_THROW_FIRE:
	{
		if (fireBalls.size() < MARIO_FIRE_MAX_FIREBALLS_NUM)
		{
			throw_fireball_start = GetTickCount64();
			isThrowingFireball = true;

			CFireBall* newFireBall = new CFireBall(x, y, nx);
			fireBalls.push_back(newFireBall);
			this->currentScene->AddObject(newFireBall);
		}
		break;
	}

	case MARIO_STATE_THROW_HAMMER:
	{
		if (hammers.size() < MARIO_FIRE_MAX_FIREBALLS_NUM)
		{
			throw_hammer_start = GetTickCount64();
			isThrowingHammer = true;

			CHammer* newHammer = new CHammer(x, y, nx);
			hammers.push_back(newHammer);
			this->currentScene->AddObject(newHammer);
		}
		break;
	}
	case MARIO_STATE_DIE:
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}
	}

	CGameObject::SetState(state);
	//DebugOutTitle(L"Mario state: %d", this->state);
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE || level == MARIO_LEVEL_HAMMER)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_RACCOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
		}
		else if (isTailWhipping)
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2 ;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

float CMario::GetBBoxWidth()
{
	float cur_bbox_l, cur_bbox_t, cur_bbox_r, cur_bbox_b;
	GetBoundingBox(cur_bbox_l, cur_bbox_t, cur_bbox_r, cur_bbox_b);
	return cur_bbox_r - cur_bbox_l;
}

float CMario::GetBBoxHeight()
{
	float cur_bbox_l, cur_bbox_t, cur_bbox_r, cur_bbox_b;
	GetBoundingBox(cur_bbox_l, cur_bbox_t, cur_bbox_r, cur_bbox_b);
	return cur_bbox_b - cur_bbox_t;
}

int CMario::GetLevel()
{
	return level;
}
