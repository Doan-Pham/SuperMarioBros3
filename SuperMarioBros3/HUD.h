#pragma once

#include "GameObject.h"

#define ID_SPRITE_HUD_BOTTOM_FRAME	83111

class CHUD : public CGameObject
{
protected:
	float width, height;
public:
	CHUD(float x, float y, float width, float height) : CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
};

