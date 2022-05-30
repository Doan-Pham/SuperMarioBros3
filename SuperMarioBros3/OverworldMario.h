#pragma once

#include "GameObject.h"

#define ID_ANI_MARIO_OVERWORLD 1291

class COverworldMario : public CGameObject
{
public:
	COverworldMario(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

