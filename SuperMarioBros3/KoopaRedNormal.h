#pragma once

#include "Koopa.h"

#define ID_ANI_KOOPA_RED_NORMAL_WALKING_LEFT					5231
#define ID_ANI_KOOPA_RED_NORMAL_WALKING_RIGHT					5232

#define ID_ANI_KOOPA_RED_NORMAL_SHELL_MOVING_UPSIDE				5233
#define ID_ANI_KOOPA_RED_NORMAL_SHELL_MOVING_DOWNSIDE			5234

#define ID_ANI_KOOPA_RED_NORMAL_SHELL_STILL_UPSIDE				5235
#define ID_ANI_KOOPA_RED_NORMAL_SHELL_STILL_DOWNSIDE			5236

#define ID_ANI_KOOPA_RED_NORMAL_SHELL_SHAKE_UPSIDE				5237
#define ID_ANI_KOOPA_RED_NORMAL_SHELL_SHAKE_DOWNSIDE			5238

class CKoopaRedNormal : public CKoopa
{
protected:

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int GetAniId();
	virtual void Render();

public:
	CKoopaRedNormal(float x, float y, const LPPLAYSCENE& currentScene) : CKoopa(x, y, currentScene) {};
};

