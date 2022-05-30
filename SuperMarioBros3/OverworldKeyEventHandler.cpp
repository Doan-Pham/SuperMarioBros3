#include "OverworldKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "OverworldScene.h"
#include "OverworldMario.h"

void COverworldKeyEventHandler::KeyState(BYTE* states)
{
}

void COverworldKeyEventHandler::OnKeyDown(int KeyCode)
{
	COverworldScene* currentScene = (COverworldScene*)CGame::GetInstance()->GetCurrentScene();
	COverworldMario* mario = (COverworldMario*)(currentScene)->GetPlayer();

	switch (KeyCode)
	{
	case DIK_LEFT:
		mario->SetState(OVERWORLD_MARIO_STATE_READY_TO_MOVE_LEFT);
		break;

	case DIK_UP:
		mario->SetState(OVERWORLD_MARIO_STATE_READY_TO_MOVE_TOP);
		break;

	case DIK_RIGHT:
		mario->SetState(OVERWORLD_MARIO_STATE_READY_TO_MOVE_RIGHT);
		break;

	case DIK_DOWN:
		mario->SetState(OVERWORLD_MARIO_STATE_READY_TO_MOVE_BOTTOM);
		break;

	case DIK_S:
		break;
	}
}

void COverworldKeyEventHandler::OnKeyUp(int KeyCode)
{
}
