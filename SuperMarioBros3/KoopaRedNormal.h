#pragma once

#include "Koopa.h"

#define ID_ANI_KOOPA_RED_NORMAL_WALKING_LEFT					5231
#define ID_ANI_KOOPA_RED_NORMAL_WALKING_RIGHT					5232

#define ID_ANI_KOOPA_RED_NORMAL_SHELL_UPSIDE_MOVING_LEFT		5233
#define ID_ANI_KOOPA_RED_NORMAL_SHELL_UPSIDE_MOVING_RIGHT		5234
#define ID_ANI_KOOPA_RED_NORMAL_SHELL_DOWNSIDE_MOVING_LEFT		5235
#define ID_ANI_KOOPA_RED_NORMAL_SHELL_DOWNSIDE_MOVING_RIGHT		5236

#define ID_ANI_KOOPA_RED_NORMAL_SHELL_UPSIDE_STILL				5237
#define ID_ANI_KOOPA_RED_NORMAL_SHELL_DOWNSIDE_STILL			5238

class CKoopaRedNormal : public CKoopa
{
protected:

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int GetAniId();
	virtual void Render();

public:
	CKoopaRedNormal(float x, float y, const LPPLAYSCENE& currentScene);
};

