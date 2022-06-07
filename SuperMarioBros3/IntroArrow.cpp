#include "IntroArrow.h"

void CIntroArrow::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ARROW_STATE_MOVE_UP:
	{
		if (current_option == 0) return;
		current_option--;
		break;
	}
		
	case ARROW_STATE_MOVE_DOWN:
	{
		if (current_option == gameOptions.size() - 1) return;
		current_option++;
		break;
	}

	case ARROW_STATE_CHOOSE_OPTION:
	{
		switch (gameOptions[current_option])
		{
		case GAME_OPTION_1_PLAYER:
			CGame::GetInstance()->InitiateSwitchScene(ID_OVERWORLD_SCENE_1);
			break;
		case GAME_OPTION_2_PLAYER:
			break;
		}
		break;
	}
	default:
		break;
	}
}
