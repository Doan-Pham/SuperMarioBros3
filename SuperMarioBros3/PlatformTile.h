#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "debug.h"

//A simple class that wraps around tiles that are already rendered. This type of platform is
//static and do pretty much nothing but standing there and blocking everything else
class CPlatformTile : public CGameObject
{
protected:
	float height, width;
public:
	CPlatformTile(float x, float y, float height, float width) :CGameObject(x, y)
	{
		this->height = height;
		this->width = width;
	}
	virtual void Render() {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

