#include "IntroKeyEventHandler.h"
#include "IntroScene.h"
#include "IntroArrow.h"

#include "debug.h"
#include "Game.h"

void CIntroKeyEventHandler::KeyState(BYTE* states)
{
}

void CIntroKeyEventHandler::OnKeyDown(int KeyCode)
{
	CIntroScene* currentScene = (CIntroScene*)CGame::GetInstance()->GetCurrentScene();
	CIntroArrow* arrow = (CIntroArrow*)(currentScene)->GetArrow();
	if (arrow->IsHidden()) return;
	switch (KeyCode)
	{
	case DIK_UP:
		arrow->SetState(ARROW_STATE_MOVE_UP);
		break;

	case DIK_DOWN:
		arrow->SetState(ARROW_STATE_MOVE_DOWN);
		break;

	case DIK_S:
		arrow->SetState(ARROW_STATE_CHOOSE_OPTION);
		break;
	}
}

void CIntroKeyEventHandler::OnKeyUp(int KeyCode)
{
}
