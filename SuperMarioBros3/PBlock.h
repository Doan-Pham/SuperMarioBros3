#pragma once

#include "Block.h"
#include "debug.h"

#define ID_ANI_P_BLOCK_NORMAL 3210
#define ID_ANI_P_BLOCK_HIT	3220

#define P_BLOCK_STATE_NORMAL 100
#define P_BLOCK_STATE_HIT_BY_MARIO 200

class CPBlock : public CBlock
{
protected:
	int IsBlocking() { return 0; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

public:
	CPBlock(float x, float y) : CBlock(x, y) { SetState(P_BLOCK_STATE_NORMAL); };
	virtual void SetState(int state);

};

