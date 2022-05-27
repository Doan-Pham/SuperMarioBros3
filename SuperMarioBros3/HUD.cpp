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

	int scores, coins, lives;
	CGame::GetInstance()->GetScoreCoinLives(scores, coins, lives);

	string scores_placeholder(HUD_SCORES_TEXT_DEFAULT_VALUE);
	string scores_string = to_string(scores);

	// Remove the unnecessary "0" in the scores_text default value
	scores_placeholder.erase(scores_placeholder.size() - scores_string.size(),scores_string.size());
	scores_text->SetTextContent(scores_placeholder + scores_string);
	scores_text->SetPosition(
		x + HUD_SCORES_TEXT_POSITION_X_OFFSET + cam_x,
		y + HUD_SCORES_TEXT_POSITION_Y_OFFSET + cam_y);

	string coins_placeholder(HUD_COINS_TEXT_DEFAULT_VALUE);
	string coins_string = to_string(coins);

	// Remove the unnecessary "0" in the coins_text default value
	coins_placeholder.erase(coins_placeholder.size() - coins_string.size(), coins_string.size());
	coins_text->SetTextContent(coins_placeholder + coins_string);
	coins_text->SetPosition(
		x + HUD_COINS_TEXT_POSITION_X_OFFSET + cam_x,
		y + HUD_COINS_TEXT_POSITION_Y_OFFSET + cam_y);

	p_meter->SetPosition(
		x + HUD_P_METER_POSITION_X_OFFSET + cam_x,
		y + HUD_P_METER_POSITION_Y_OFFSET + cam_y);

	vector<int> card_types = CGame::GetInstance()->GetCardsAcquired();
	for (int i = 0; i < card_types.size();i++)
	{
		cardsAcquired[i]->SetCurrentType(card_types[i]);
	};

	time_text->SetPosition(
		x + HUD_TIME_TEXT_POSITION_X_OFFSET + cam_x,
		y + HUD_TIME_TEXT_POSITION_Y_OFFSET + cam_y);
}

void CHUD::Render()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	// We need to use the floor() method on these coordinates first or else inside the Sprites::Draw() 
	// method later, the flooring then will make coordinates wrong and create shaking when rendering 
	cam_x = (FLOAT)floor(cam_x);
	cam_y = (FLOAT)floor(cam_y);

	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	CSprites::GetInstance()->Get(ID_SPRITE_HUD_BOTTOM_FRAME)->Draw(x + cam_x , y + cam_y);

	scores_text->Render();
	time_text->Render();
	coins_text->Render();
	p_meter->Render();

	float current_card_x = x + HUD_CARDS_POSITION_X_OFFSET + cam_x;
	float card_y = y + HUD_CARDS_POSITION_Y_OFFSET + cam_y;

	for (CCard* card : cardsAcquired)
	{
		card->SetPosition(current_card_x, card_y);
		card->Render();
		current_card_x += CARD_BBOX_WIDTH + 2;
	}
	//DebugOut(L"hud_x : %0.5f; hud_y : %0.5f, cam_x : %0.5f, cam_y : %0.5f \n", x, y, cam_x, cam_y);
}
