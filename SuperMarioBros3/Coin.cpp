#include "Coin.h"

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == COIN_STATE_BECOME_BRICK &&
		GetTickCount64() - become_brick_start > BECOME_BRICK_TIMEOUT)
	{
		SetState(COIN_STATE_NORMAL);
	}
}

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	
	if (state == COIN_STATE_NORMAL)
		animations->Get(ID_ANI_COIN)->Render(x, y);

	else if (state == COIN_STATE_BECOME_BRICK)
		animations->Get(ID_ANI_BRICK_GLASS_NORMAL)->Render(x, y);
	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == COIN_STATE_NORMAL)
	{
		l = x - COIN_BBOX_WIDTH / 2;
		t = y - COIN_BBOX_HEIGHT / 2;
		r = l + COIN_BBOX_WIDTH;
		b = t + COIN_BBOX_HEIGHT;
	}
	else
	{
		l = x - BLOCK_BBOX_WIDTH / 2;
		t = y - BLOCK_BBOX_HEIGHT / 2;
		r = l + BLOCK_BBOX_WIDTH;
		b = t + BLOCK_BBOX_HEIGHT;
	}
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_BECOME_BRICK:
		become_brick_start = GetTickCount64();
		break;
	}
}
