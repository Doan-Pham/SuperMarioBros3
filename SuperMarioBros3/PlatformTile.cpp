#include "PlatformTile.h"

void CPlatformTile::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width/2;
	t = y - height/2;
	r = l + width;
	b = t + height;
	//DebugOutTitle(L"x: %0.5f, y: %0.5f, l:%0.5f,t:%0.5f r: %0.5f, b: %0.5f", x, y,l,t, r, b);
}
