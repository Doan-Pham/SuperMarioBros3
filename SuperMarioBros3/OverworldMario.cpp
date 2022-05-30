#include "OverworldMario.h"
#include "Animations.h"

#include "debug.h"
#include "OverworldScene.h"

COverworldMario::COverworldMario(float x, float y) : CGameObject(x, y)
{
	current_node = NULL;
	destination_node = NULL;
	nx = 0;
	ny = 0;
}

void COverworldMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (destination_node == NULL || current_node->GetId() == destination_node->GetId()) return;
	x += vx * dt;
	y += vy * dt;
}

void COverworldMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MARIO_OVERWORLD)->Render(x, y);
	DebugOutTitle(L"mario_x : %0.5f, mario-y : %0.5f",x, y);
}

void COverworldMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void COverworldMario::SetState(int state)
{
	switch (state)
	{
	case OVERWORLD_MARIO_STATE_STILL:
	{
		vx = 0;
		vy = 0;
		nx = 0;
		ny = 0;
		break;
	}

	case OVERWORLD_MARIO_STATE_READY_TO_MOVE_LEFT:
	{
		if (state == OVERWORLD_MARIO_STATE_MOVING) return;
		
		if (COverworldScene::GetNode(current_node->GetConnectedNodeLeft()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeLeft());
			nx = -1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
		}

		break;
	}

	case OVERWORLD_MARIO_STATE_READY_TO_MOVE_TOP:
	{
		if (state == OVERWORLD_MARIO_STATE_MOVING) return;

		if (COverworldScene::GetNode(current_node->GetConnectedNodeTop()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeLeft());
			ny = -1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
		}

		break;
	}

	case OVERWORLD_MARIO_STATE_READY_TO_MOVE_RIGHT:
	{
		if (state == OVERWORLD_MARIO_STATE_MOVING) return;

		if (COverworldScene::GetNode(current_node->GetConnectedNodeRight()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeRight());
			nx = 1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
		}

		break;
	}

	case OVERWORLD_MARIO_STATE_READY_TO_MOVE_BOTTOM:
	{
		if (state == OVERWORLD_MARIO_STATE_MOVING) return;

		if (COverworldScene::GetNode(current_node->GetConnectedNodeBottom()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeBottom());
			ny = 1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
		}

		break;
	}
	case OVERWORLD_MARIO_STATE_MOVING:
	{
		vx = nx * OVERWORLD_MARIO_SPEED_X;
		vy = ny * OVERWORLD_MARIO_SPEED_Y;
		break;
	}
	default:
		break;
	}

	CGameObject::SetState(state);
}
