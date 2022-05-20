#include "PlantGreenFire.h"
void CPlantGreenFire::Render()
{
	if (state == PLANT_STATE_HIDING) return;

	if (state == PLANT_STATE_APPEARING || state == PLANT_STATE_DISAPPEARING)
		CAnimations::GetInstance()->Get(GetPlantAniId())->Render(x, y);
	else
		CSprites::GetInstance()->Get(GetPlantSpriteId())->Draw(x, y);

	//RenderBoundingBox();
	//DebugOut(L"plant_x : %0.5f, plant_y : %0.5f \n", x, y);
}

int CPlantGreenFire::GetPlantSpriteId()
{
	if (relative_nx_to_mario > 0)
	{
		if (relative_ny_to_mario > 0) return ID_SPRITE_PLANT_GREEN_FIRE_OPEN_TOP_RIGHT;
		else return ID_SPRITE_PLANT_GREEN_FIRE_OPEN_BOTTOM_RIGHT;
	}
	else
	{
		if (relative_ny_to_mario > 0) return ID_SPRITE_PLANT_GREEN_FIRE_OPEN_TOP_LEFT;
		else return ID_SPRITE_PLANT_GREEN_FIRE_OPEN_BOTTOM_LEFT;
	}
	return -1;
}

int CPlantGreenFire::GetPlantAniId()
{
	if (relative_nx_to_mario > 0)
	{
		if (relative_ny_to_mario > 0) return ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_TOP_RIGHT;
		else return ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_BOTTOM_RIGHT;
	}
	else
	{
		if (relative_ny_to_mario > 0) return ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_TOP_LEFT;
		else return ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_BOTTOM_LEFT;
	}
	return -1;
}