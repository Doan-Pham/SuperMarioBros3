#include "PlantRedFire.h"


CPlantRedFire::CPlantRedFire(float x, float y) : CGameObject(x,y)
{
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
	CAnimations::GetInstance()->Get(ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_LEFT)->Render(x, y);
	RenderBoundingBox();
	DebugOut(L"plant_x : %0.5f, plant_y : %0.5f \n", x, y);
}



void CPlantRedFire::SetState(int state)
{
}
