#pragma once
#include "GameObject.h"
#include "Sprites.h"

#define ID_SPRITE_INTRO_TREE		84301

class CTree : public CGameObject
{
protected:

public:
	CTree(float x, float y) :CGameObject(x, y){}

	void Render()
	{
		CSprites::GetInstance()->Get(ID_SPRITE_INTRO_TREE)->Draw(x, y);
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};
