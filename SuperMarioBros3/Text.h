#pragma once

#include "GameObject.h"
class CText : public CGameObject
{
protected:
	vector<char> characters;
public:
	CText(float x, float y, vector<char> characters) : CGameObject(x, y)
	{
		this->characters = characters;
	}
	void Render() {};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};

