#include "PBlock.h"
#include "Mario.h"

void CPBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == P_BLOCK_STATE_NORMAL)
		animations->Get(ID_ANI_P_BLOCK_NORMAL)->Render(x, y);
	else
		animations->Get(ID_ANI_P_BLOCK_HIT)->Render(x, y);
	//RenderBoundingBox();
	//DebugOutTitle(L"Brick Question Mark y: %0.5f, vy: %0.5f, ay: %0.5f  \n",
	//	y, vy, ay);
}

void CPBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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


