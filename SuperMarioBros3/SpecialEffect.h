#include "GameObject.h"

// A simple class wrapping around a special effect
class CSpecialEffect : public CGameObject
{
protected:
	float ax, ay;

	int animationId = -1;
	int animationTime;
	ULONGLONG effect_start;

public:
	CSpecialEffect(float x, float y, float vx, float vy, float ax, float ay, int animationId, int animationTime) :CGameObject(x, y) 
	{
		this->vx = vx;
		this->vy = vy;
		this->ax = ax;
		this->ay = ay;
		this->animationId = animationId;
		this->animationTime = animationTime;

		effect_start = -1;
	};
	void Render() 
	{ 
		// We need the timer check in case the effect is not purged yet despite being deleleted
		// (when mario's tranforming)
		if (animationId > 0 && GetTickCount64() - effect_start <= animationTime)
			CAnimations::GetInstance()->Get(animationId)->Render(x, y);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		if (effect_start == -1) effect_start = GetTickCount64();
		else if (GetTickCount64() - effect_start > animationTime) this->Delete();
		vx += ax * dt;
		vy += ay * dt;

		x += vx * dt;
		y += vy * dt;
	};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};

	// This method is for manually starting the effect's timer, in case the automatic timer-start mechanism
	// is not used (when Update() method is not called while mario's transforming)
	void StartEffectTimer() { effect_start = GetTickCount64(); }
	int IsBlocking() { return 0; }
};
