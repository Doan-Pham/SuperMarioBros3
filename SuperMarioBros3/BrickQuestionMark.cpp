#include "BrickQuestionMark.h"
#include "Mario.h"

CBrickQuestionMark::CBrickQuestionMark(float x, float y, bool isHidingItem) 
	: CBlock(x, y)
{
	y_original = this->y;
	this->isHidingItem = isHidingItem;

	isHitByMario = false;
	isContentGiven = false;
	SetState(BRICK_STATE_NORMAL);
}

void CBrickQuestionMark::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == BRICK_STATE_NORMAL)
		animations->Get(ID_ANI_BRICK_QUESTIONMARK_NORMAL)->Render(x, y);
	else
		animations->Get(ID_ANI_BRICK_QUESTIONMARK_HIT)->Render(x, y);
	//RenderBoundingBox();
	//DebugOutTitle(L"Brick Question Mark y: %0.5f, vy: %0.5f, ay: %0.5f  \n",
	//	y, vy, ay);
}

void CBrickQuestionMark::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BRICK_STATE_HIT_BY_MARIO)
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

void CBrickQuestionMark::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_NORMAL:
		ay = 0;
		break;

	case BRICK_STATE_HIT_BY_MARIO:
		if (!isHitByMario)
		{
			vy = -BRICK_QUESTION_MARK_BOUNCE_SPEED;
			ay = BRICK_QUESTION_MARK_GRAVITY;
			isHitByMario = true;
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
	}
}


