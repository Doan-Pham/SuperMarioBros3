#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "debug.h"

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

	void Render() { RenderBoundingBox(); };
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

