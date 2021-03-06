#include "OverworldNode.h"

COverworldNode::COverworldNode(int id, int nodeType, float x, float y,
	int nodeLeft, int nodeTop, int nodeRight, int nodeBottom, int playSceneId) : CGameObject(x, y)
{
	this->id = id;
	this->nodeType = nodeType;
	this->connectedNodeLeft = nodeLeft;
	this->connectedNodeTop = nodeTop;
	this->connectedNodeRight = nodeRight;
	this->connectedNodeBottom = nodeBottom;
	this->playscene_id = playSceneId;
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
