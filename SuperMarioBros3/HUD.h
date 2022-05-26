#pragma once

#include "GameObject.h"
#include "Text.h"

#define ID_SPRITE_HUD_BOTTOM_FRAME	83111

#define HUD_SCORES_DEFAULT_VALUE				"0000000"
#define HUD_SCORES_POSITION_X_OFFSET_CENTER		-90
#define HUD_SCORES_POSITION_Y_OFFSET_CENTER		-1

#define HUD_TIME_DEFAULT_VALUE					"000"
#define HUD_TIME_POSITION_X_OFFSET_CENTER		-10
#define HUD_TIME_POSITION_Y_OFFSET_CENTER		-1

class CHUD : public CGameObject
{
protected:
	float width, height;
	CText* scores_text;
	CText* time_text;

public:
	CHUD(float x, float y, float width, float height) : CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
		float hud_l, hud_t, hud_r, hud_b;
		GetBoundingBox(hud_l, hud_t, hud_r, hud_b);
		
		scores_text = new CText(
			x - HUD_SCORES_POSITION_X_OFFSET_CENTER,
			y - HUD_SCORES_POSITION_Y_OFFSET_CENTER,
			HUD_SCORES_DEFAULT_VALUE);
		scores_text->UnHide();

		time_text = new CText(
			x - HUD_TIME_POSITION_X_OFFSET_CENTER,
			y - HUD_TIME_POSITION_Y_OFFSET_CENTER,
			HUD_TIME_DEFAULT_VALUE);
		time_text->UnHide();
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt);
	void Render();
};

