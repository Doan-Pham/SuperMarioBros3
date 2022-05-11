#include "PlantRedFire.h"


CPlantRedFire::CPlantRedFire(float x, float y) : CGameObject(x,y)
{
	disappear_start = -1;
	appearing_destination_y = y - PLANT_BBOX_HEIGHT;
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
	// TODO: Need to find some way to avoid having to reference so many general objects
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mario_x, mario_y;
	mario->GetPosition(mario_x, mario_y);
	ULONGLONG now = GetTickCount64();
	if (state == PLANT_STATE_HIDING && now - disappear_start > PLANT_TIME_BETWEEN_APPEARANCES)
	{
		SetState(PLANT_STATE_APPEARING);
	}
	if (y + vy * dt < appearing_destination_y)
		vy = (appearing_destination_y - y) / dt;
	else if (y + vy * dt == appearing_destination_y)
		SetState(PLANT_STATE_FIRING);

	y += vy * dt;
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
	if (this->state == PLANT_STATE_FIRING) return ID_SPRITE_PLANT_RED_FIRE_OPEN_TOP_LEFT;
	else return ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_LEFT;
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
		vy = 0;
		break;

	case PLANT_STATE_DISAPPEARING:
		disappear_start = GetTickCount64();
		vy = PLANT_MOVING_SPEED;
		break;
	}
}
