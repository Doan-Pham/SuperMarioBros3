#pragma once
#include "GameObject.h"


#define CARD_TYPE_MUSHROOM	1
#define CARD_TYPE_STAR		2
#define CARD_TYPE_FLOWER	3

#define CARD_BBOX_WIDTH		16
#define CARD_BBOX_HEIGHT	16

#define ID_SPRITE_CARD_MUSHROOM_NOT_TAKEN	44211
#define ID_SPRITE_CARD_STAR_NOT_TAKEN		44221
#define ID_SPRITE_CARD_FLOWER_NOT_TAKEN		44231

#define SWITCH_TYPE_TIMEOUT	100

class CCard : public CGameObject
{
protected:
	vector<int> card_types;
	int current_type_index;
	ULONGLONG switch_type_start;
public:
	CCard(float x, float y) : CGameObject(x, y)
	{
		current_type_index = -1;
		card_types.push_back(CARD_TYPE_MUSHROOM);
		card_types.push_back(CARD_TYPE_STAR);
		card_types.push_back(CARD_TYPE_FLOWER);
		switch_type_start = -1;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int GetCurrentType() { return card_types[current_type_index]; }
};

