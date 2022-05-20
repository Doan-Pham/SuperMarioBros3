#include "MushroomUp.h"

void CMushroomUp::Render()
{
	if (state != MUSHROOM_STATE_HIDING)
		CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM_UP_MOVING)->Render(x, y);
}
