#pragma once
#include "GameObject.h"

class COverworldNode : public CGameObject
{
protected:
	int id;
	int nodeType;
	
	// Each node can be connected to 4 other nodes in 4 directions
	int connectedNodeLeft;
	int connectedNodeTop;
	int connectedNodeRight;
	int connectedNodeBottom;

	// In case this node leads to a playscene
	int playscene_id;

public:
	COverworldNode(int id, int nodeType, float x, float y, 
		int nodeLeft, int nodeTop, int nodeRight, int nodeBottom, int playSceneId);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int GetConnectedNodeLeft() { return connectedNodeLeft; }
	int GetConnectedNodeTop() { return connectedNodeTop; }
	int GetConnectedNodeRight() { return connectedNodeRight; }
	int GetConnectedNodeBottom() { return connectedNodeBottom; }
	int GetId() { return id; }

	bool IsLeadingToPlayScene() { return playscene_id != -1; }
	int GetPlaySceneId() { return playscene_id; }
	void SetPlaySceneId(int id) { playscene_id = id; }
};

