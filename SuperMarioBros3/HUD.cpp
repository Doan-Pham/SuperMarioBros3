#include "HUD.h"
#include "Sprites.h"
#include "Game.h"
#include "debug.h"
#include <string>

void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - width / 2;
	top = y - height / 2;
	right = left + width;
	bottom = top + height;
}

void CHUD::Update(DWORD dt)
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	scores_text->SetPosition(
		x + HUD_SCORES_POSITION_X_OFFSET_CENTER + cam_x,
		y + HUD_SCORES_POSITION_Y_OFFSET_CENTER + cam_y);

	time_text->SetPosition(
		x + HUD_TIME_POSITION_X_OFFSET_CENTER + cam_x,
		y + HUD_TIME_POSITION_Y_OFFSET_CENTER + cam_y);
}

void CHUD::Render()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	// We need to use the floor() method on these coordinates first or else inside the Sprites::Draw() 
	// method later, the flooring then will make coordinates and create shaking when rendering 
	cam_x = (FLOAT)floor(cam_x);
	cam_y = (FLOAT)floor(cam_y);

	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	CSprites::GetInstance()->Get(ID_SPRITE_HUD_BOTTOM_FRAME)->Draw(x + cam_x , y + cam_y);

	scores_text->Render();
	time_text->Render();

	DebugOut(L"hud_x : %0.5f; hud_y : %0.5f, cam_x : %0.5f, cam_y : %0.5f \n", x, y, cam_x, cam_y);
}
