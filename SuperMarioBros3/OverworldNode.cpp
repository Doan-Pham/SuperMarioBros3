#include "OverworldNode.h"

COverworldNode::COverworldNode(int id, int nodeType, float x, float y,
	int nodeLeft, int nodeTop, int nodeRight, int nodeBottom) : CGameObject(x, y)
{
	this->id = id;
	this->nodeType = nodeType;
	this->connectedNodeLeft = nodeLeft;
	this->connectedNodeTop = nodeTop;
	this->connectedNodeRight = nodeRight;
	this->connectedNodeBottom = nodeBottom;
}

void COverworldNode::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void COverworldNode::Render()
{
}

void COverworldNode::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
