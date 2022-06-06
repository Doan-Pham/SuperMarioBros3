#pragma once
#include "GameObject.h"
#include "Sprites.h"

#define ID_SPRITE_MISC_INTRO_TITLE		84201
#define ID_ANI_MISC_INTRO_TITLE			8420

#define TITLE_SPEED_Y				0.1f

class CGameTitle : public CGameObject
{
protected:
	float destination_y;
	bool isFlashing;
public:
	CGameTitle(float x, float y) :CGameObject(x, y)
	{
		this->vy = TITLE_SPEED_Y;
		destination_y = CGame::GetInstance()->GetBackBufferHeight() / 3;
		isFlashing = false;
	}

	void Render()
	{
		if (!isFlashing)
			CSprites::GetInstance()->Get(ID_SPRITE_MISC_INTRO_TITLE)->Draw(x, y);
		else
			CAnimations::GetInstance()->Get(ID_ANI_MISC_INTRO_TITLE)->Render(x, y);
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		y += vy * dt;
		if (y >= destination_y)
		{
			y = destination_y;
			vy = 0.0f;
		};
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	void SetFlashing() { isFlashing = true; }
};
