#pragma once

#include "Block.h"
#include "debug.h"

#define ID_ANI_P_BLOCK_NORMAL 3210
#define ID_ANI_P_BLOCK_HIT	3220

#define P_BLOCK_STATE_NORMAL 100
#define P_BLOCK_STATE_HIT_BY_MARIO 200

#define P_BLOCK_AFTER_HIT_BBOX_WIDTH 16
#define P_BLOCK_AFTER_HIT_BBOX_HEIGHT 6

class CPBlock : public CBlock
{
protected:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
public:
	CPBlock(float x, float y) : CBlock(x, y) { SetState(P_BLOCK_STATE_NORMAL); };
	int IsBlocking() { return state == P_BLOCK_STATE_NORMAL; }
	virtual void SetState(int state);

};

