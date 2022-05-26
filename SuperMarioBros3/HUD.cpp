#include "HUD.h"
#include "Sprites.h"
#include "Game.h"

void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - width / 2;
	top = y - height / 2;
	right = left + width;
	bottom = top + height;
}

void CHUD::Render()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	CSprites::GetInstance()->Get(ID_SPRITE_HUD_BOTTOM_FRAME)->Draw(x + cam_x , y + cam_y);
}
