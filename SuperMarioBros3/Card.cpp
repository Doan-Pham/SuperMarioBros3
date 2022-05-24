#include "Card.h"
#include "Sprites.h"
void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ULONGLONG now = GetTickCount64();
	if (current_type_index == -1)
	{
		current_type_index = 0;
		switch_type_start = now;
	}
	else if (now - switch_type_start > SWITCH_TYPE_TIMEOUT)
	{
		current_type_index++;
		switch_type_start = now;
		if (current_type_index == card_types.size()) current_type_index = 0;
	}
}
void CCard::Render()
{
	// Sometimes card is not updated in Update() method of map, so this check is needed
	if (current_type_index == -1) return;

	switch (card_types[current_type_index])
	{
	case CARD_TYPE_MUSHROOM:
		CSprites::GetInstance()->Get(ID_SPRITE_CARD_MUSHROOM_NOT_TAKEN)->Draw(x, y);
		break;
	case CARD_TYPE_STAR:
		CSprites::GetInstance()->Get(ID_SPRITE_CARD_STAR_NOT_TAKEN)->Draw(x, y);
		break;
	case CARD_TYPE_FLOWER:
		CSprites::GetInstance()->Get(ID_SPRITE_CARD_FLOWER_NOT_TAKEN)->Draw(x, y);
		break;
	}
}

void CCard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CARD_BBOX_WIDTH / 2;
	top = y - CARD_BBOX_HEIGHT / 2;
	right = left + CARD_BBOX_WIDTH;
	bottom = top + CARD_BBOX_HEIGHT;
}
