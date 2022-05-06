#pragma once

#include "PlatformTile.h"
class CPlatformGhost : public CPlatformTile
{

protected:
	virtual int IsBlocking() { return 0; }
public:
	CPlatformGhost(float x, float y, float height, float width)
		:CPlatformTile(x, y, height, width) {};

};

