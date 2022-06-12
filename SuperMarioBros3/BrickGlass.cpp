#include "BrickGlass.h"
#include "Mario.h"
#include "SpecialEffectManager.h"

CBrickGlass::CBrickGlass(float x, float y, bool isHidingUpMushroom, bool isHidingPBlock)
	: CBlock(x, y)
{
	y_original = this->y;
	this->isHidingUpMushroom = isHidingUpMushroom;
	this->isHidingPBlock = isHidingPBlock;
	become_coin_start = -1;
	isHitByMario = false;
	isContentGiven = false;
	SetState(BRICK_STATE_NORMAL);
}

void CBrickGlass::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == BRICK_STATE_NORMAL)
		animations->Get(ID_ANI_BRICK_GLASS_NORMAL)->Render(x, y);
	else if (state == BRICK_STATE_BECOME_COIN)
		animations->Get(ID_ANI_COIN)->Render(x, y);
	else
		animations->Get(ID_ANI_BRICK_GLASS_HIT)->Render(x, y);
	//RenderBoundingBox();
	//DebugOut(L"Brick glass x : %d state: %d , ani : %d  \n", x, state, aniId);
	//	y, vy, ay);
}

void CBrickGlass::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BRICK_STATE_BECOME_COIN &&
		GetTickCount64() - become_coin_start > BECOME_COIN_TIMEOUT)
	{
		SetState(BRICK_STATE_NORMAL);
	}

	if ((isHidingUpMushroom || isHidingPBlock) && state == BRICK_STATE_HIT_BY_MARIO)
	{
		vy += ay * dt;

		//Adjust vy so that the brick won't go past the original postion when falling down after
		//bouncing up
		if (y + vy * dt > y_original)
		{
			vy = (y_original - y) / dt;
		}
		y += vy * dt;

		//DebugOutTitle(L"Brick Question Mark y: %0.5f, vy: %0.5f, ay: %0.5f  \n",
		//	y, vy, ay);

		if (y == y_original)
		{
			SetState(BRICK_STATE_GIVE_CONTENT);
		}
	}
}

void CBrickGlass::SetState(int state)
{

	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_NORMAL:
		ay = 0;
		break;

	case BRICK_STATE_HIT_BY_MARIO:
		if ((isHidingUpMushroom || isHidingPBlock))
		{
			if (!isHitByMario)
			{
				vy = -BRICK_GLASS_BOUNCE_SPEED;
				ay = BRICK_GLASS_GRAVITY;
				isHitByMario = true;
			}
		}
		else
		{
			CSpecialEffectManager::CreateSpecialEffect(x, y, EFFECT_TYPE_BRICK_GLASS_BROKEN);
			Delete();
		}

		break;

	case BRICK_STATE_GIVE_CONTENT:
		vy = 0;
		ay = 0;
		if (!isContentGiven)
		{
			if (hiddenItem == nullptr)
			{
				CGame::GetInstance()->UpdateCoins(this->GetCoinsGivenWhenHit());
				CGame::GetInstance()->UpdateScores(this->GetScoresGivenWhenHit());
			}
			else
				hiddenItem->SetState(ITEM_STATE_APPEARING);
			isContentGiven = true;
		}
		break;

	case BRICK_STATE_BECOME_COIN:
		// Brick_glass that contains items still remains normal so the p-block can stand on it
		if (isHidingUpMushroom || isHidingPBlock)
			isBlocking = true;
		else
		{
			become_coin_start = GetTickCount64();
			isBlocking = false;
		}
		break;
	}
}


