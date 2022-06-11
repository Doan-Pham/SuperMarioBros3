#include "GameObject.h"

// A simple class wrapping around a special effect
class CSpecialEffect : public CGameObject
{
protected:
	int animationId = -1;
	int animationTime;
	ULONGLONG effect_start;

public:
	CSpecialEffect(float x, float y, float vx, float vy, int animationId, int animationTime) :CGameObject(x, y) 
	{
		this->vx = vx;
		this->vy = vy;
		this->animationId = animationId;
		this->animationTime = animationTime;
		effect_start = -1;
	};
	void Render() 
	{ 
		if (animationId > 0) CAnimations::GetInstance()->Get(animationId)->Render(x, y);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		if (effect_start == -1) effect_start = GetTickCount64();
		else if (GetTickCount64() - effect_start > animationTime) this->Delete();
		y += vy * dt;
		x += vx * dt;
	};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};

	int IsBlocking() { return 0; }
};
