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

public:
	COverworldNode(int id, int nodeType, float x, float y, 
		int nodeLeft, int nodeTop, int nodeRight, int nodeBottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int GetConnectedNodeLeft() { return connectedNodeLeft; }
	int GetConnectedNodeTop() { return connectedNodeTop; }
	int GetConnectedNodeRight() { return connectedNodeRight; }
	int GetConnectedNodeBottom() { return connectedNodeBottom; }
	int GetId() { return id; }
};

