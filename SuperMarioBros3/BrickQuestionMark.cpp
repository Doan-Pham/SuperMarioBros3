#include "BrickQuestionMark.h"

void CBrickQuestionMark::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBrickQuestionMark::Update(DWORD dt)
{
}
