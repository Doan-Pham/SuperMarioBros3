#pragma once
#include "Item.h"

#define CARD_SPEED_Y	0.08f

#define CARD_TYPE_MUSHROOM	1
#define CARD_TYPE_STAR		2
#define CARD_TYPE_FLOWER	3

#define CARD_BBOX_WIDTH		16
#define CARD_BBOX_HEIGHT	16

#define ID_SPRITE_CARD_MUSHROOM_NOT_TAKEN	44211
#define ID_SPRITE_CARD_STAR_NOT_TAKEN		44221
#define ID_SPRITE_CARD_FLOWER_NOT_TAKEN		44231

#define ID_ANI_CARD_MUSHROOM_SPINNING		4421
#define ID_ANI_CARD_STAR_SPINNING			4422
#define ID_ANI_CARD_FLOWER_SPINNING			4423

#define ID_SPRITE_CARD_MUSHROOM_TAKEN		44214
#define ID_SPRITE_CARD_STAR_TAKEN			44224
#define ID_SPRITE_CARD_FLOWER_TAKEN			44234

#define SWITCH_TYPE_TIMEOUT		100

#define CARD_STATE_NOT_TAKEN	100
#define CARD_STATE_SPINNING		200
#define CARD_STATE_TAKEN		300

class CCard : public CItem
{
protected:
	vector<int> card_types;
	int current_type_index;
	ULONGLONG switch_type_start;
public:
	CCard(float x, float y) : CItem(x, y)
	{
		current_type_index = -1;

		// TODO: Add these card types as constructor parameters
		card_types.push_back(CARD_TYPE_MUSHROOM);
		card_types.push_back(CARD_TYPE_STAR);
		card_types.push_back(CARD_TYPE_FLOWER);
		switch_type_start = -1;
		
		SetState(CARD_STATE_NOT_TAKEN);
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	int IsBlocking() { return 0; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int GetCurrentType() { return card_types[current_type_index]; }

	void SetState(int state);
};

