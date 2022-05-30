#pragma once

#include "GameObject.h"
#include "OverworldNode.h"

#define ID_ANI_MARIO_OVERWORLD		1291

#define OVERWORLD_MARIO_SPEED_X		0.1f
#define OVERWORLD_MARIO_SPEED_Y		0.1f

#define OVERWORLD_MARIO_STATE_STILL						100

#define OVERWORLD_MARIO_STATE_READY_TO_MOVE_LEFT		200
#define OVERWORLD_MARIO_STATE_READY_TO_MOVE_TOP			300
#define OVERWORLD_MARIO_STATE_READY_TO_MOVE_RIGHT		400
#define OVERWORLD_MARIO_STATE_READY_TO_MOVE_BOTTOM		500

#define OVERWORLD_MARIO_STATE_MOVING					600

class COverworldMario : public CGameObject
{
protected:
	int ny;
	// These 2 will be used to decide where mario is and will be!
	COverworldNode* current_node;
	COverworldNode* destination_node;

public:
	COverworldMario(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	void SetCurrentNode(COverworldNode* current_node) { this->current_node = current_node; }
};

