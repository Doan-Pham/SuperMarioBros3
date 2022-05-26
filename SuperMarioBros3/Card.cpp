#include "Card.h"
#include "Sprites.h"
#include "Animations.h"

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == CARD_STATE_NOT_TAKEN)
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
	else if (state == CARD_STATE_SPINNING)
	{
		y += vy * dt;
	}
}
void CCard::Render()
{
	// Sometimes card is not updated in Update() method of map, so this check is needed
	if (current_type_index == -1) return;

	switch (card_types[current_type_index])
	{
	case CARD_TYPE_MUSHROOM:
		if (state == CARD_STATE_NOT_TAKEN)
			CSprites::GetInstance()->Get(ID_SPRITE_CARD_MUSHROOM_NOT_TAKEN)->Draw(x, y);
		else if (state == CARD_STATE_SPINNING)
			CAnimations::GetInstance()->Get(ID_ANI_CARD_MUSHROOM_SPINNING)->Render(x, y);
		else 
			CSprites::GetInstance()->Get(ID_SPRITE_CARD_MUSHROOM_TAKEN)->Draw(x, y);

		break;

	case CARD_TYPE_STAR:
		if (state == CARD_STATE_NOT_TAKEN)
			CSprites::GetInstance()->Get(ID_SPRITE_CARD_STAR_NOT_TAKEN)->Draw(x, y);
		else if (state == CARD_STATE_SPINNING)
			CAnimations::GetInstance()->Get(ID_ANI_CARD_STAR_SPINNING)->Render(x, y);
		else
			CSprites::GetInstance()->Get(ID_SPRITE_CARD_STAR_TAKEN)->Draw(x, y);

		break;
	case CARD_TYPE_FLOWER:
		if (state == CARD_STATE_NOT_TAKEN)
			CSprites::GetInstance()->Get(ID_SPRITE_CARD_FLOWER_NOT_TAKEN)->Draw(x, y);
		else if (state == CARD_STATE_SPINNING)
			CAnimations::GetInstance()->Get(ID_ANI_CARD_FLOWER_SPINNING)->Render(x, y);
		else 
			CSprites::GetInstance()->Get(ID_SPRITE_CARD_FLOWER_TAKEN)->Draw(x, y);

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

void CCard::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CARD_STATE_NOT_TAKEN:
		vy = 0;
		break;
	case CARD_STATE_SPINNING:
		vy = -CARD_SPEED_Y;
		break;
	case CARD_STATE_TAKEN:
		break;
	}
}
