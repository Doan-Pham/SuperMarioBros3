#include "BrickQuestionMark.h"

CBrickQuestionMark::CBrickQuestionMark(float x, float y) : CBrick(x, y)
{
	y_original = this->y;
	isHiddenItemAppeared = false;
	hiddenItemToDropIndex = 0;
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

		if (y == y_original && !isHiddenItemAppeared)
		{
			if (hiddenItems.size() == 0)
			{
				CGame::GetInstance()->UpdateCoins(this->GetCoinsGivenWhenHit());
				CGame::GetInstance()->UpdateScores(this->GetScoresGivenWhenHit());
			}
			else
				hiddenItems[hiddenItemToDropIndex]->SetState(ITEM_STATE_APPEARING);

			isHiddenItemAppeared = true;
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBrickQuestionMark::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == BRICK_STATE_HIT_BY_MARIO) return;
	if (!(dynamic_cast<CMario*>(e->obj))) return;
	if (e->ny < 0 && e->nx == 0)
	{
		SetState(BRICK_STATE_HIT_BY_MARIO);
		CMario* mario = dynamic_cast<CMario*>(e->obj);

		// If mario's level is equal to or higher than mario_big, then the brick returns leaf
		// else, the brick returns the mushroom_big
		if (hiddenItems.size() > 1)
		{
			hiddenItemToDropIndex = mario->GetLevel() >= MARIO_LEVELS_BASELINE;
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
		vy = -BRICK_QUESTION_MARK_BOUNCE_SPEED;
		ay = BRICK_QUESTION_MARK_GRAVITY;
		break;
	}
}


