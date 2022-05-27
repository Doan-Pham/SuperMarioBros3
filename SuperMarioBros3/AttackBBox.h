#pragma once

#include "GameObject.h"

class CPlayScene;
typedef CPlayScene* LPPLAYSCENE;

// A bbox that can be attached to another object and work like a weapon to kill anything that object 
// wants. In this case, it's the mario_raccoon's tail
class CAttackBBox : public CGameObject
{
protected:
	float width, height;

	// TODO: Need a better way than this to interact with brick_question_mark
	const LPPLAYSCENE currentScene;

	virtual void OnNoCollision(DWORD dt) {};
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


public:
	CAttackBBox(float x, float y, float vx, float vy, float width, float height,
		const LPPLAYSCENE currentScene);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void SetDirection(int nx) { this->nx = nx; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render() { RenderBoundingBox(); };
};

