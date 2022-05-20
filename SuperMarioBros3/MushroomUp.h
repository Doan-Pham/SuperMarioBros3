#pragma once

#include "MushroomBig.h"

#define ID_ANI_MUSHROOM_UP_MOVING	4120	

class CMushroomUp : public CMushroomBig
{
protected:
	virtual void Render();
public:
	CMushroomUp(float x, float y) : CMushroomBig(x, y) {};
};

