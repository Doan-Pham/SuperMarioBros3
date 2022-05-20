#include "PBlock.h"
#include "Mario.h"

void CPBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == P_BLOCK_STATE_NORMAL)
		animations->Get(ID_ANI_P_BLOCK_NORMAL)->Render(x, y);
	else
		//Adjust the p-block's y after hit to keep it on top of the brick
		animations->Get(ID_ANI_P_BLOCK_HIT)->Render
		(x, y + (BLOCK_BBOX_HEIGHT - P_BLOCK_AFTER_HIT_BBOX_HEIGHT)/2);

	//RenderBoundingBox();
	//DebugOutTitle(L"Brick Question Mark y: %0.5f, vy: %0.5f, ay: %0.5f  \n",
	//	y, vy, ay);
}

void CPBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CPBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == P_BLOCK_STATE_NORMAL)
	{
		l = x - BLOCK_BBOX_WIDTH / 2;
		t = y - BLOCK_BBOX_HEIGHT / 2;
		r = l + BLOCK_BBOX_WIDTH;
		b = t + BLOCK_BBOX_HEIGHT;
	}
	else
	{
		l = x - P_BLOCK_AFTER_HIT_BBOX_WIDTH / 2;
		t = y - P_BLOCK_AFTER_HIT_BBOX_HEIGHT / 2;
		r = l + P_BLOCK_AFTER_HIT_BBOX_WIDTH;
		b = t + P_BLOCK_AFTER_HIT_BBOX_HEIGHT;
	}
}

void CPBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case P_BLOCK_STATE_NORMAL:
		break;

	case P_BLOCK_STATE_HIT_BY_MARIO:
		break;
	}
}


