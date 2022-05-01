#include "BrickQuestionMark.h"

void CBrickQuestionMark::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (!isContentTaken)
		animations->Get(ID_ANI_BRICK_QUESTIONMARK)->Render(x, y);
	else
		animations->Get(ID_ANI_TEST)->Render(x, y);
	//RenderBoundingBox();
}

void CBrickQuestionMark::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += ay * dt;

	if (y + vy*dt != y_original)
	{
		vy = y_original - y;
	}
	//y += vy * dt;
	DebugOutTitle(L"Brick Question Mark y: %0.5f, vy: %0.5f, ay: %0.5f  \n",
		y, vy, ay);
	//if (abs(vx) > abs(maxVx)) vx = maxVx;
	//if (isContentTaken) return;
	//DebugOutTitle(L"Brick Question Mark state is: %d \n",
	//	isContentTaken);
	//vy += ay * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBrickQuestionMark::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (isContentTaken) return;
	if (!(dynamic_cast<CMario*>(e->obj))) return;
	if (e->ny > 0 && e->nx == 0)
	{
		this->vy = -BRICK_QUESTION_MARK_BOUNCE_SPEED;
		this->ay = BRICK_QUESTION_MARK_GRAVITY;
	}
		
	isContentTaken = true;
	DebugOutTitle(L"Brick Question Mark state is: %d \n",
		isContentTaken);
}

void CBrickQuestionMark::OnNoCollision(DWORD dt)
{
	//if (isContentTaken) return;
	y += vy * dt;
}


