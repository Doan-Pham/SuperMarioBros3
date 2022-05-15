#pragma once

#include "GameObject.h"

// A bbox that can be attached to another object and work like a weapon to kill anything that object 
// wants. In this case, it's the mario_raccoon's tail
class CAttackBBox : public CGameObject
{
protected:
	int width, height;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& botto);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() { RenderBoundingBox(); };

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) {};
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


public:
	CAttackBBox(float x, float y, float vx, float vy, int width, int height);
};

