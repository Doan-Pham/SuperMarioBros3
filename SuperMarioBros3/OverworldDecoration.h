#pragma once

#include "GameObject.h"

#define ID_ANI_OVERWORLD_DECORATION_WORLD_1		8451

class COverworldDecoration : public CGameObject
{
protected:

public:
	COverworldDecoration(float x, float y) : CGameObject(x, y) {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState(int state) {};
};

