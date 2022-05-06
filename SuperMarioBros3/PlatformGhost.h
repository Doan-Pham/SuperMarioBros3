#pragma once

#include "PlatformTile.h"
class CPlatformGhost : public CPlatformTile
{
public:
	CPlatformGhost(float x, float y, float height, float width)
		:CPlatformTile(x, y, height, width) {};

};

