#include "HUD.h"
#include "Sprites.h"
#include "Game.h"
#include "debug.h"

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

	scores->SetPosition(
		x + HUD_SCORES_POSITION_X_OFFSET_CENTER + cam_x, 
		y + HUD_SCORES_POSITION_Y_OFFSET_CENTER + cam_y);

	scores->Render();

	time->SetPosition(
		x + HUD_TIME_POSITION_X_OFFSET_CENTER + cam_x,
		y + HUD_TIME_POSITION_Y_OFFSET_CENTER + cam_y);

	time->Render();
	DebugOut(L"hud_x : %0.5f; hud_y : %0.5f, cam_x : %0.5f, cam_y : %0.5f \n", x, y, cam_x, cam_y);
}
