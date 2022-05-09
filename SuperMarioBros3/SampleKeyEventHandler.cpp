#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON )
		{ 
			float mario_vx, mario_vy;
			mario->GetSpeed(mario_vx, mario_vy);
			if (mario->IsPMeterFullyCharged())
				mario->SetState(MARIO_STATE_FLY);

			else if (mario_vy == 0)
				mario->SetState(MARIO_STATE_JUMP);
			else if (mario_vy > 0)
				mario->SetState(MARIO_STATE_TAIL_WAGGING);
			//if (mario_vy <= 0)
				
			//else 
			//	mario->SetState(MARIO_STATE_TAIL_WAGGING);
				
		}
			
		else
			mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
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
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		// Release jump will make mario fall down immediately, which is not the case for mario raccoon
		if (!(mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->IsPMeterFullyCharged()))
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;

	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

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