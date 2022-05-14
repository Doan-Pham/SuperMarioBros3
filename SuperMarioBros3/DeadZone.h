#pragma once
#include "GameObject.h"
#include "debug.h"

class CDeadZone : public CGameObject
{
protected:
	float height, width;
	int IsBlocking() { return 0; }
	void Render() { RenderBoundingBox(); };
public:
	CDeadZone(float x, float y, float height, float width) :CGameObject(x, y)
	{
		this->height = height;
		this->width = width;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b)
	{
		l = x - width / 2;
		t = y - height / 2;
		r = l + width;
		b = t + height;
	};
};

