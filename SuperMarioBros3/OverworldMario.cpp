#include "OverworldMario.h"
#include "Animations.h"

#include "debug.h"
#include "OverworldScene.h"

COverworldMario::COverworldMario(float x, float y) : CGameObject(x, y)
{
	current_node = NULL;
	destination_node = NULL;
	SetState(OVERWORLD_MARIO_STATE_STILL);
}

void COverworldMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (destination_node == NULL || current_node->GetId() == destination_node->GetId()) return;

	if (state == OVERWORLD_MARIO_STATE_MOVING)
	{
		float dest_node_x, dest_node_y;
		destination_node->GetPosition(dest_node_x, dest_node_y);

		// Simple check to see if mario's position has gone past the destination
		if (((dest_node_x - x ) * nx < 0) || ((dest_node_y - y) * ny < 0))
		{
			x = dest_node_x;
			y = dest_node_y;
			current_node = destination_node;
			SetState(OVERWORLD_MARIO_STATE_STILL);
		}
		DebugOutTitle(L"mario_x : %0.5f, mario-y : %0.5f, dest-x : %0.5f, dest-y : %0.5f",
			x, y, dest_node_x, dest_node_y);
	}
	x += vx * dt;
	y += vy * dt;
}

void COverworldMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MARIO_OVERWORLD)->Render(x, y);
	//DebugOutTitle(L"mario_x : %0.5f, mario-y : %0.5f",x, y);
	//DebugOutTitle(L"mario_state %i", state);
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
		if (this->state == OVERWORLD_MARIO_STATE_MOVING) return;
		
		if (COverworldScene::GetNode(current_node->GetConnectedNodeLeft()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeLeft());
			nx = -1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
			return;
		}

		break;
	}

	case OVERWORLD_MARIO_STATE_READY_TO_MOVE_TOP:
	{
		if (this->state == OVERWORLD_MARIO_STATE_MOVING) return;

		if (COverworldScene::GetNode(current_node->GetConnectedNodeTop()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeTop());
			ny = -1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
			return;
		}

		break;
	}

	case OVERWORLD_MARIO_STATE_READY_TO_MOVE_RIGHT:
	{
		if (this->state == OVERWORLD_MARIO_STATE_MOVING) return;

		if (COverworldScene::GetNode(current_node->GetConnectedNodeRight()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeRight());
			nx = 1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
			return;
		}

		break;
	}

	case OVERWORLD_MARIO_STATE_READY_TO_MOVE_BOTTOM:
	{
		if (this->state == OVERWORLD_MARIO_STATE_MOVING) return;

		if (COverworldScene::GetNode(current_node->GetConnectedNodeBottom()) != NULL)
		{
			this->destination_node = COverworldScene::GetNode(current_node->GetConnectedNodeBottom());
			ny = 1;
			SetState(OVERWORLD_MARIO_STATE_MOVING);
			return;
		}

		break;
	}
	case OVERWORLD_MARIO_STATE_MOVING:
	{
		vx = nx * OVERWORLD_MARIO_SPEED_X;
		vy = ny * OVERWORLD_MARIO_SPEED_Y;
		break;
	}

	case OVERWORLD_MARIO_STATE_SWITCH_SCENE:
	{
		if (current_node->IsLeadingToPlayScene())
			CGame::GetInstance()->InitiateSwitchScene(current_node->GetPlaySceneId());
		else return;
		break;
	}
	default:
		break;
	}

	CGameObject::SetState(state);
}
