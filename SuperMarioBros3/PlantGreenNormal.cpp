#include "PlantGreenNormal.h"

CPlantGreenNormal::CPlantGreenNormal(float x, float y) : CGameObject(x, y)
{
	hide_start = -1;
	appear_start = -1;

	relative_nx_to_mario = -1;
	relative_ny_to_mario = -1;

	isMarioInFireZone = false;
	appearing_destination_y = y - PLANT_BBOX_HEIGHT;
	disappearing_destination_y = y;

	SetState(PLANT_STATE_HIDING);
}

void CPlantGreenNormal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PLANT_BBOX_WIDTH / 2;
	top = y - PLANT_BBOX_HEIGHT / 2;
	right = left + PLANT_BBOX_WIDTH;
	bottom = top + PLANT_BBOX_HEIGHT;
}

void CPlantGreenNormal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		// Adjust plant's vy so it never goes past the destination
		if (y + vy * dt < appearing_destination_y)
			vy = (appearing_destination_y - y) / dt;

		else if (y + vy * dt == appearing_destination_y && now - appear_start > PLANT_APPEARING_TIMEOUT)
			SetState(PLANT_STATE_DISAPPEARING);
		y += vy * dt;
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
	//DebugOutTitle(L"mario_x : %0.5f, x: %0.5f, |mario_x - x|: %0.5f", mario_x, x, abs(mario_x - x));

}

void CPlantGreenNormal::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_PLANT_GREEN_NORMAL)->Render(x, y);
	RenderBoundingBox();
	//DebugOut(L"plant_x : %0.5f, plant_y : %0.5f \n", x, y);
}


void CPlantGreenNormal::SetState(int state)
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
		appear_start = GetTickCount64();
		vy = -PLANT_MOVING_SPEED;
		//DebugOutTitle(L"Plant : appearing");
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
