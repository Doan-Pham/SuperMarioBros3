#pragma once

#include "GameObject.h"

#define ITEM_STATE_HIDING 100
#define ITEM_STATE_APPEARING 200
#define ITEM_STATE_MOVING 300

class CItem : public CGameObject
{
public:
	CItem(float x, float y) : CGameObject(x, y) {}
	virtual void Render() = 0;
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
	virtual int IsBlocking() { return 0; }
	virtual void SetState(int state) {};

	// This is higher than brick's priority so bricks can cover them
	virtual int GetRenderPriority() { return 30; }
};

