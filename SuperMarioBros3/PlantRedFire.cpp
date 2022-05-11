#include "PlantRedFire.h"


CPlantRedFire::CPlantRedFire(float x, float y) : CGameObject(x,y)
{
	hide_start = -1;
	aim_start = -1;
	fire_start = -1;

	isMarioInFireZone = false;
	appearing_destination_y = y - PLANT_BBOX_HEIGHT;
	disappearing_destination_y = y;

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

	if (state == PLANT_STATE_HIDING 
		&& (PLANT_APPEARING_ZONE_MIN < abs(mario_x - x) && abs(mario_x - x) < PLANT_APPEARING_ZONE_MAX)
		&& now - hide_start > PLANT_TIME_BETWEEN_APPEARANCES)
	{
		SetState(PLANT_STATE_APPEARING);
	}

	if (state == PLANT_STATE_APPEARING)
	{
		if (y + vy * dt < appearing_destination_y)
			vy = (appearing_destination_y - y) / dt;
		else if (y + vy * dt == appearing_destination_y)
			SetState(PLANT_STATE_AIMING);
		y += vy * dt;
	}

	if (state == PLANT_STATE_AIMING)
	{
		if (now - aim_start < PLANT_AIMING_TIMEOUT)
		{
			if (abs(mario_x - x) < PLANT_FIRING_ZONE_MAX) isMarioInFireZone = true;		
		}
		else
		{
			if (isMarioInFireZone) SetState(PLANT_STATE_FIRING);
			else if (now - aim_start > PLANT_AIMING_TIMEOUT + PLANT_FIRING_TIMEOUT) 
				SetState(PLANT_STATE_DISAPPEARING);
		}	
	}

	if (state == PLANT_STATE_FIRING)
	{
		if (now - fire_start > PLANT_FIRING_TIMEOUT)
			SetState(PLANT_STATE_DISAPPEARING);
	}

	if (state == PLANT_STATE_DISAPPEARING)
	{
		if (y + vy * dt > disappearing_destination_y)
			vy = (disappearing_destination_y - y) / dt;
		else if (y + vy * dt == disappearing_destination_y)
			SetState(PLANT_STATE_HIDING);
		y += vy * dt;
	}
	//DebugOutTitle(L"mario_x : %0.5f, x: %0.5f, |mario_x - x|: %0.5f", mario_x, x, abs(mario_x - x));

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
		hide_start = GetTickCount64();
		break;

	case PLANT_STATE_APPEARING:
		vy = -PLANT_MOVING_SPEED;
		DebugOutTitle(L"Plant : appearing");
		break;

	case PLANT_STATE_AIMING:
		vy = 0;
		aim_start = GetTickCount64();
		DebugOutTitle(L"Plant : aiming");
		break;

	case PLANT_STATE_FIRING:
		fire_start = GetTickCount64();
		DebugOutTitle(L"Plant : firing");
		break;

	case PLANT_STATE_DISAPPEARING:
		vy = PLANT_MOVING_SPEED;	
		DebugOutTitle(L"Plant : disappearing");
		break;
	}
}
