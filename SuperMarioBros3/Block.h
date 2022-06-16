#pragma once

#include "GameObject.h"

#define BLOCK_BBOX_WIDTH 16
#define BLOCK_BBOX_HEIGHT 16

class CBlock : public CGameObject {
public:
	CBlock(float x, float y) : CGameObject(x, y) {}
	virtual void Render() = 0;
	virtual void Update(DWORD dt) {}
	virtual int IsBlocking() { return 1; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};