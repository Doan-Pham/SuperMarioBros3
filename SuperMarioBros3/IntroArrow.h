#pragma once
#include "GameObject.h"
#include "Sprites.h"

#define ID_SPRITE_INTRO_ARROW		84401

#define ARROW_STATE_MOVE_UP				100
#define ARROW_STATE_MOVE_DOWN			200
#define ARROW_STATE_CHOOSE_OPTION		300

#define ARROW_MOVE_DISTANCE_Y			16

#define GAME_OPTION_1_PLAYER			1
#define GAME_OPTION_2_PLAYER			2

// TODO: Find some way to not hardcode this
#define ID_OVERWORLD_SCENE_1			10

class CIntroArrow : public CGameObject
{
protected:
	vector<int> gameOptions;
	int current_option;
	float y_original;

public:
	CIntroArrow(float x, float y) :CGameObject(x, y) 
	{
		current_option = 0;
		y_original = y;
		gameOptions.push_back(GAME_OPTION_1_PLAYER);
		gameOptions.push_back(GAME_OPTION_2_PLAYER);
	}

	void Render()
	{
		CSprites::GetInstance()->Get(ID_SPRITE_INTRO_ARROW)->Draw(x, y);
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
	{
		y = y_original + current_option * ARROW_MOVE_DISTANCE_Y;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	void SetState(int state);
};
