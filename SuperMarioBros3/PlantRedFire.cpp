#include "PlantRedFire.h"


CPlantRedFire::CPlantRedFire(float x, float y) : CGameObject(x,y)
{
	hide_start = -1;
	aim_start = -1;
	fire_start = -1;

	relative_nx_to_mario = -1;
	relative_ny_to_mario = -1;

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

void CPlantRedFire::GetPositionRelativeToMario(int mario_x, int mario_y, int& nx, int& ny)
{
	if (mario_x < x) nx = -1;
	else nx = 1;
	if (mario_y < y) ny = 1;
	else ny = -1;

	this->fireshot_dest_x = mario_x;
	this->fireshot_dest_y = mario_y;
}

void CPlantRedFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// TODO: Need to find some way to avoid having to reference so many general objects
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mario_x, mario_y;
	mario->GetPosition(mario_x, mario_y);

	GetPositionRelativeToMario(mario_x, mario_y, relative_nx_to_mario, relative_ny_to_mario);

	ULONGLONG now = GetTickCount64();

	if (state == PLANT_STATE_HIDING 
		&& (PLANT_APPEARING_ZONE_MIN < abs(mario_x - x) && abs(mario_x - x) < PLANT_APPEARING_ZONE_MAX)
		&& now - hide_start > PLANT_TIME_BETWEEN_APPEARANCES)
	{
		SetState(PLANT_STATE_APPEARING);
	}

	if (state == PLANT_STATE_APPEARING)
	{
		// Adjust plant's vy so it never goes past the destination
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
			// Aiming timeout! Shoots fireshots if mario is in the firezone or wait for a bit then
			// disappear
			if (isMarioInFireZone)
			{
				SetState(PLANT_STATE_FIRING);
				isMarioInFireZone = false;
			}

			else if (now - aim_start > PLANT_AIMING_TIMEOUT + PLANT_FIRING_TIMEOUT) 
				SetState(PLANT_STATE_DISAPPEARING);
		}	
	}

	if (state == PLANT_STATE_FIRING)
	{
		// After plants fires its shot, it waits a bit before disappearing
		if (now - fire_start > PLANT_FIRING_TIMEOUT)
			SetState(PLANT_STATE_DISAPPEARING);
	}

	if (state == PLANT_STATE_DISAPPEARING)
	{
		// Adjust plant's vy so it never goes past the destination
		if (y + vy * dt > disappearing_destination_y)
			vy = (disappearing_destination_y - y) / dt;
		else if (y + vy * dt == disappearing_destination_y)
			SetState(PLANT_STATE_HIDING);
		y += vy * dt;
	}
	DebugOutTitle(L"mario_x : %0.5f, x: %0.5f, |mario_x - x|: %0.5f", mario_x, x, abs(mario_x - x));

}

void CPlantRedFire::Render()
{
	if (state == PLANT_STATE_HIDING) return;

	if (state == PLANT_STATE_APPEARING || state == PLANT_STATE_DISAPPEARING)
		CAnimations::GetInstance()->Get(GetPlantAniId())->Render(x, y);
	else
		CSprites::GetInstance()->Get(GetPlantSpriteId())->Draw(x, y);

	//RenderBoundingBox();
	//DebugOut(L"plant_x : %0.5f, plant_y : %0.5f \n", x, y);
}

int CPlantRedFire::GetPlantSpriteId()
{
	if (relative_nx_to_mario > 0)
	{
		if (relative_ny_to_mario > 0) return ID_SPRITE_PLANT_RED_FIRE_OPEN_TOP_RIGHT;
		else return ID_SPRITE_PLANT_RED_FIRE_OPEN_BOTTOM_RIGHT;
	}
	else
	{
		if (relative_ny_to_mario > 0) return ID_SPRITE_PLANT_RED_FIRE_OPEN_TOP_LEFT;
		else return ID_SPRITE_PLANT_RED_FIRE_OPEN_BOTTOM_LEFT;
	}
	return -1;
}

int CPlantRedFire::GetPlantAniId()
{
	if (relative_nx_to_mario > 0)
	{
		if (relative_ny_to_mario > 0) return ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_RIGHT;
		else return ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_BOTTOM_RIGHT;
	}
	else
	{
		if (relative_ny_to_mario > 0) return ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_TOP_LEFT;
		else return ID_ANI_PLANT_RED_FIRE_OPEN_CLOSE_BOTTOM_LEFT;
	}
	return -1;
}

void CPlantRedFire::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLANT_STATE_HIDING:
	{
		vy = 0;
		hide_start = GetTickCount64();
		break;
	}

	case PLANT_STATE_APPEARING:
	{
		vy = -PLANT_MOVING_SPEED;
		//DebugOutTitle(L"Plant : appearing");
		break;

	}

	case PLANT_STATE_AIMING:
	{
		vy = 0;
		aim_start = GetTickCount64();
		//DebugOutTitle(L"Plant : aiming");
		break;

	}

	case PLANT_STATE_FIRING:
	{
		fire_start = GetTickCount64();
		CFireShot* fireBall = new CFireShot(x, y - PLANT_BBOX_HEIGHT/4, 
			fireshot_dest_x, fireshot_dest_y);

		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->AddObject(fireBall);
		//DebugOutTitle(L"Plant : firing");
		break;
	}

	case PLANT_STATE_DISAPPEARING:
	{
		vy = PLANT_MOVING_SPEED;
		//DebugOutTitle(L"Plant : disappearing");
		break;
	}
	}
}
