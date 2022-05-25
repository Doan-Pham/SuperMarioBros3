#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_COURSE_CLEAR) return;

	CMap* map = (CMap*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetCurrentMap();
	int mapWidth, mapHeight, mapTileWidth, mapTileHeight;
	map->GetSize(mapWidth, mapHeight);
	map->GetTileSize(mapTileWidth, mapTileHeight);

	float mapLeftEdge = 0;
	float mapTopEdge = 0;

	// Adjust the right, bottom edges to avoid seeing empty tiles
	float mapRightEdge = (float)(mapWidth * mapTileWidth - 8);
	float mapBottomEdge = (float)(mapHeight * mapTileHeight - 8);

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{
			float mario_vx, mario_vy;
			mario->GetSpeed(mario_vx, mario_vy);

			// P-meter is full in 2 cases: mario's flying, and mario's has finished flying and falling
			// but has not touched the ground
			if (mario->IsPMeterFullyCharged())
			{
				if (!mario->IsTrulyFalling()) mario->SetState(MARIO_STATE_FLY);
				else mario->SetState(MARIO_STATE_TAIL_WAGGING);
			}
			else if (mario_vy == 0)
				mario->SetState(MARIO_STATE_JUMP);
			else if (mario_vy > 0)
				mario->SetState(MARIO_STATE_TAIL_WAGGING);
		}

		else
			mario->SetState(MARIO_STATE_JUMP);
		break;

	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->SetState(MARIO_STATE_TAIL_WHIPPING);

		else if (mario->GetLevel() == MARIO_LEVEL_FIRE)
			mario->SetState(MARIO_STATE_THROW_FIRE);

		else if (mario->GetLevel() == MARIO_LEVEL_HAMMER)
			mario->SetState(MARIO_STATE_THROW_HAMMER);

		//mario->NotifyPMeterAKeyHit();
		break;

	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;

	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;

	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;

	case DIK_4:
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;

	case DIK_5:
		mario->SetLevel(MARIO_LEVEL_HAMMER);
		break;
	case DIK_G:
	{
		float mario_x, mario_y;
		mario->GetPosition(mario_x, mario_y);
		mario->SetPosition(mario_x + min(500, mapRightEdge - mario_x), mario_y - 20);
	}

		break;
	case DIK_F:
	{
		float mario_x, mario_y;
		mario->GetPosition(mario_x, mario_y);
		mario->SetPosition(mario_x + max(-500, mapLeftEdge - mario_x), mario_y - 20);
		break;
	}

	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_COURSE_CLEAR) return;

	switch (KeyCode)
	{
	case DIK_S:
		// Release jump will make mario fall down immediately, which is not always the case for 
		// mario raccoon: when he's flying, and when he's falling
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{
			if (!mario->IsTrulyFalling() && !mario->IsFlying())
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
		}
		else mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;

	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;

	case DIK_A:
		if (mario->IsHoldingShell()) mario->KickHeldShell();

		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_COURSE_CLEAR) return;

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}