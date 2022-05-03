#pragma once

#include "GameObject.h"


class CItem : public CGameObject
{
public:
	CItem(float x, float y) : CGameObject(x, y) {}
	virtual void Render() = 0;
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
	virtual int IsBlocking() { return 0; }
	virtual void SetState(int state) {};
	virtual int GetScoresGivenWhenHit() { return 0; }
	virtual int GetCoinsGivenWhenHit() { return 0; }
};

