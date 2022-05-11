#include "PlantRedFire.h"


CPlantRedFire::CPlantRedFire(float x, float y) : CGameObject(x,y)
{
	SetState(PLANT_STATE_HIDING);
}

void CPlantRedFire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PLANT_BBOX_WIDTH / 2;
	top = y - PLANT_BBOX_HEIGHT / 2;
	right = left + PLANT_BBOX_WIDTH;
	bottom = top + PLANT_BBOX_HEIGHT;
}

void CPlantRedFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CPlantRedFire::Render()
{
	if (state == PLANT_STATE_HIDING) return;
	int aniSpriId;
	CAnimations::GetInstance()->Get(ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_LEFT)->Render(x, y);
	RenderBoundingBox();
	//DebugOut(L"plant_x : %0.5f, plant_y : %0.5f \n", x, y);
}

int CPlantRedFire::GetPlantAniSpriId()
{
	int aniSprId;
	if (this->state == PLANT_STATE_FIRING) aniSprId = ID_SPRITE_PLANT_RED_FIRE_OPEN_TOP_LEFT;
	else aniSprId = ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_LEFT;
}

void CPlantRedFire::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLANT_STATE_HIDING:
		vy = 0;
		break;

	case PLANT_STATE_APPEARING:
		vy = -PLANT_MOVING_SPEED;
		break;

	case PLANT_STATE_FIRING:
		break;

	case PLANT_STATE_DISAPPEARING:
		vy = PLANT_MOVING_SPEED;
		break;
	}
}
