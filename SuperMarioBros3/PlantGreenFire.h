#pragma once
#include "PlantRedFire.h"
#define ID_SPRITE_PLANT_GREEN_FIRE_OPEN_TOP_LEFT				53411
#define ID_SPRITE_PLANT_GREEN_FIRE_OPEN_TOP_RIGHT				53421
#define ID_SPRITE_PLANT_GREEN_FIRE_OPEN_BOTTOM_LEFT				53431
#define ID_SPRITE_PLANT_GREEN_FIRE_OPEN_BOTTOM_RIGHT			53441

#define ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_TOP_LEFT				5341
#define ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_TOP_RIGHT			5342
#define ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_BOTTOM_LEFT			5343
#define ID_ANI_PLANT_GREEN_FIRE_OPEN_CLOSE_BOTTOM_RIGHT			5344


class CPlantGreenFire : public CPlantRedFire
{
protected:
	virtual void Render();

	virtual int GetPlantSpriteId();
	virtual int GetPlantAniId();
public:
	CPlantGreenFire(float x, float y) : CPlantRedFire(x, y) {};
};

