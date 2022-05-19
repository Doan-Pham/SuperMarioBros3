#pragma once

#include "Koopa.h"


#define ID_ANI_KOOPA_GREEN_WING_WALKING_LEFT					5221
#define ID_ANI_KOOPA_GREEN_WING_WALKING_RIGHT					5222

#define ID_ANI_KOOPA_GREEN_WING_SHELL_MOVING_UPSIDE				5223
#define ID_ANI_KOOPA_GREEN_WING_SHELL_MOVING_DOWNSIDE			5224

#define ID_ANI_KOOPA_GREEN_WING_HOPPING_LEFT					5225
#define ID_ANI_KOOPA_GREEN_WING_HOPPING_RIGHT					5226 

#define ID_ANI_KOOPA_GREEN_WING_SHELL_STILL_UPSIDE				5227
#define ID_ANI_KOOPA_GREEN_WING_SHELL_STILL_DOWNSIDE			5228

class CKoopaGreenWing : public CKoopa
{
protected:
	bool isOnPlatform;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int GetAniId();
	virtual void Render();
public:
	CKoopaGreenWing(float x, float y, const LPPLAYSCENE& currentScene) : CKoopa(x, y, currentScene) 
	{
		SetState(KOOPA_STATE_HOPPING);
		isOnPlatform = false;
	};
};

