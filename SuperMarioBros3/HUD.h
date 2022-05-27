#pragma once

#include "GameObject.h"
#include "Text.h"
#include "PMeter.h"

#define ID_SPRITE_HUD_BOTTOM_FRAME	83111

#define HUD_SCORES_TEXT_DEFAULT_VALUE			"0000000"

// The offset of scores_text based on the center coordinates the whole HUD
#define HUD_SCORES_TEXT_POSITION_X_OFFSET		-90
#define HUD_SCORES_TEXT_POSITION_Y_OFFSET		-1

// There is a blank space in this value because in original game, the value for coins doesn't have
// a "0" character before single-digit values (from 1-9)
#define HUD_COINS_TEXT_DEFAULT_VALUE			" 0"
#define HUD_COINS_TEXT_POSITION_X_OFFSET		-2
#define HUD_COINS_TEXT_POSITION_Y_OFFSET		-9

#define HUD_TIME_TEXT_DEFAULT_VALUE				"000"
#define HUD_TIME_TEXT_POSITION_X_OFFSET			-10
#define HUD_TIME_TEXT_POSITION_Y_OFFSET			-1

#define HUD_P_METER_POSITION_X_OFFSET			-90
#define HUD_P_METER_POSITION_Y_OFFSET			-10

class CHUD : public CGameObject
{
protected:
	float width, height;

	// TODO: Need a more organized one than this
	CText* scores_text;
	CText* time_text;
	CText* coins_text;

	CPMeter* p_meter;
public:
	CHUD(float x, float y, float width, float height) : CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
		float hud_l, hud_t, hud_r, hud_b;
		GetBoundingBox(hud_l, hud_t, hud_r, hud_b);
		
		scores_text = new CText(
			x - HUD_SCORES_TEXT_POSITION_X_OFFSET,
			y - HUD_SCORES_TEXT_POSITION_Y_OFFSET,
			HUD_SCORES_TEXT_DEFAULT_VALUE);
		scores_text->UnHide();

		time_text = new CText(
			x - HUD_TIME_TEXT_POSITION_X_OFFSET,
			y - HUD_TIME_TEXT_POSITION_Y_OFFSET,
			HUD_TIME_TEXT_DEFAULT_VALUE);
		time_text->UnHide();

		coins_text = new CText(
			x - HUD_COINS_TEXT_POSITION_X_OFFSET,
			y - HUD_COINS_TEXT_POSITION_Y_OFFSET,
			HUD_COINS_TEXT_DEFAULT_VALUE);
		coins_text->UnHide();
	};

	void SetPMeter(CPMeter* p_meter) { this->p_meter = p_meter; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt);
	void Render();
};

