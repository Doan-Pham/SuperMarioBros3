#pragma once

#include "Brick.h"

#define ID_ANI_BRICK_QUESTIONMARK 10000

class CBrickQuestionMark : public CBrick
{
public:
	virtual void Render();
	virtual void Update(DWORD dt);
};

